/**
 * File: main.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "triangle.h"
#include "point.h"
#include "vect.h"
#include "vertex.h"
#include "color.h"
#include "quat.h"
#include "axes.h"
#include "camera.h"
#include "trig_helper.h"

typedef struct {
    const Triangle_t *const * triangles;
    const Camera_t *cam;
    double frame_width;
    double frame_height;
    int img_width;
    int img_height;
} Scene_t;

typedef struct {
    Triangle_t triangles[2*12];
} TriRing12_t;

void TriRing12_cfg(TriRing12_t *const pThis, const Point_t * pCenter, const Vect_t *pFirst, const Vect_t *pUp, const double height_angle)
{
    unsigned int i;
    Quat_t rot;
    Vect_t tptr, bptr;
    Vect_t hinge;
    Point_t pt;
    Color_t col;
    Vertex_t verts[24];

    Color_t colors[3];
    Color_cfg(&colors[0], 255, 0, 0);
    Color_cfg(&colors[1], 0, 255, 0);
    Color_cfg(&colors[2], 0, 0, 255);

    //Pointer to the first vertex in the top row.
    Vect_copy(&tptr, pFirst);

    //Axis to rotate the top pointer around to get the bottom pointer.
    Vect_cross(&hinge, pFirst, pUp);

    //Quat to rotate around the hinge.
    Quat_rotation(&rot, &hinge, height_angle);

    //Pointer to the first vertex in the bottom row.
    Quat_rotateVect(&rot, &bptr, &tptr);
    Quat_rotation(&rot, pUp, TWO_PI/24.0);
    Quat_rotateVect(&rot, &bptr, &bptr);
    
    //Quat to rotate around the up axis.
    Quat_rotation(&rot, pUp, TWO_PI/12.0);

    for(i=0; i<12; i++)
    {
        //fill in the top row of vertices
        Point_translate(&pt, pCenter, &tptr);
        Color_copy(&col, &(colors[i%3]));
        Vertex_cfg(&(verts[2*i]), &pt, &col);

        //And the bottom row.
        Point_translate(&pt, pCenter, &bptr);
        Color_copy(&col, &(colors[(i+1)%3]));
        Vertex_cfg(&(verts[2*i+1]), &pt, &col);

        //Rotate around to the next triangle.
        Quat_rotateVect(&rot, &tptr, &tptr);
        Quat_rotateVect(&rot, &bptr, &bptr);
    }

    for(i=0; i<12; i++)
    {
        //And the triangle.
        Triangle_cfg(&(pThis->triangles[2*i]), &(verts[2*i]), &(verts[2*((i+1)%12)]), &(verts[2*i+1]));
        Triangle_cfg(&(pThis->triangles[2*i+1]), &(verts[2*i+1]), &(verts[2*((i+1)%12)]), &(verts[2*((i+1)%12)+1]));
    }
}

typedef struct {
    Point_t top_left;
    Vect_t step_down;
    Vect_t step_right;
} Frame_t;

static void Frame_cfg(Frame_t *pThis, const Scene_t *const scene)
{
    Point_t eye;
    Vect_t pov;
    Vect_t up;
    Vect_t right;

    //Get the eye, pov, and up vector from the camera.
    Camera_getEye(scene->cam, &eye);
    Camera_getPov(scene->cam, &pov);
    Camera_getUp(scene->cam, &up);

    //Create the vector pointing right as the cross product of up and pov.
    Vect_cross(&right, &pov, &up);
    
    //Now make sure up is really up, i.e., perpendicular to both pov and right
    // (because right is a cross product of pov, we know right is perpendicular to pov.
    // The cross product of right and pov will be perp to both, so all three will be
    // mutually perpendicular.
    Vect_cross(&up, &right, &pov);

    //Make up be half the height of the frame, and right be half the width.
    Vect_setMag(&up, (scene->frame_height)*0.5);
    Vect_setMag(&right, (scene->frame_width)*0.5);

    //Get a point in the top-left corner of the frame by starting at the eye,
    // translating to the center of the frame with pov, then translating to the top-center
    // of the frame with up, and then translating back to the top-left corner with right.
    Point_translate(&(pThis->top_left), &eye, &pov);
    Point_translate(&(pThis->top_left), &(pThis->top_left), &up);
    Point_translateBack(&(pThis->top_left), &(pThis->top_left), &right);

    //Now get scaled vectors that represent a single step along the grid of the frame,
    // in each direction.
    Vect_scale(&(pThis->step_down), &up, -1.0 / (((double)(scene->img_height)) * 0.5));
    Vect_scale(&(pThis->step_right), &right, 1.0 / (((double)(scene->img_width)) * 0.5));
}

static void render_scene(GdkPixbuf *const pixbuf, const Scene_t *const scene)
{
    unsigned int i, j;
    Frame_t frame;
    double min_dist;
    Color_t render_color;
    const Triangle_t *const *pTriangle;
    guchar *scanline;
    guchar *pix;
    Point_t eye;

    //The point we cast rays through.
    Point_t pt;

    //The ray we cast through it.
    Vect_t ray;

    //The point at the beginning of the row.
    Point_t row_start;

    const int width = scene->img_width;
    const int height = scene->img_height;
    const int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    guchar *const pixels = gdk_pixbuf_get_pixels(pixbuf);

    // Set Up the Frame
    Frame_cfg(&frame, scene);


    //// Render the Triangles ////

    //Get the camera eye
    Camera_getEye(scene->cam, &eye);
    
    //First row starts with the top-left corner.
    Point_copy(&row_start, &(frame.top_left));
    
    //Conveniently, so does the raster data.
    scanline = pixels;

    //Iterate over each row, starting at the top and going down.
    for(j=0; j<height; j++) {

        //Our point will walk along the row, starting with the first point in the row.
        Point_copy(&pt, &row_start);

        //Same with the scan line.
        pix = scanline;
        
        //Iterate over the pixels in the row, moving left to right.
        for(i=0; i<width; i++) {

            //Get the vector from the eye to the current point.
            Point_displacement(&ray, &eye, &pt);

            //Find which triangle it intersect withs closest.
            min_dist = INFINITY;
            Color_cfg(&render_color, 0, 0, 0);
            pTriangle = scene->triangles;
            for(pTriangle = scene->triangles; *(pTriangle) != NULL; pTriangle++)
            {
                min_dist = Triangle_rayCast(*pTriangle, &render_color, min_dist, &pt, &ray);
            }

            //Set the pixel in the GdkPixbuf.
            pix[0] = render_color.r;
            pix[1] = render_color.g;
            pix[2] = render_color.b;

            //Step to the next pixel.
            Point_translate(&pt, &pt, &(frame.step_right));
            pix += 3;
        }

        //Step to the next row.
        Point_translate(&row_start, &row_start, &(frame.step_down));
        scanline += rowstride;
    }

}


/**
 *
 * Coordinates:
 *
 * Note that this is a right-handed coordinate system, which may be counterintuitive in some cases.
 * X is left and right, Y is up and down, Z is in and out (of the screen), but positive X
 * is **to the left**, positive Y is up, and positive Z is into the screen.
 */
static gboolean draw(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    const GdkPixbuf *const pixbuf = (const GdkPixbuf*)(data);
    const int width = gdk_pixbuf_get_width(pixbuf);
    const int height = gdk_pixbuf_get_height(pixbuf);

    //Draw the pixbuf to the window.
    gdk_draw_pixbuf(widget->window, NULL, pixbuf, 0, 0, 0, 0, width, height, GDK_RGB_DITHER_NONE, 0, 0);

    return TRUE;
}

void show_scene(const Scene_t *const scene)
{
    GtkWidget *window;
    const int width = scene->img_width;
    const int height = scene->img_height;

    //Create our pix buffer.
    GdkPixbuf * pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    g_assert(gdk_pixbuf_get_n_channels(pixbuf) == 3);
    g_assert(gdk_pixbuf_get_width(pixbuf) == width);
    g_assert(gdk_pixbuf_get_height(pixbuf) == height);

    //Draw on it.
    render_scene(pixbuf, scene);

    //Create the GTK window.
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), scene->img_width, scene->img_height);
    gtk_window_set_title (GTK_WINDOW (window), "Ray Trace");

    //Connect it to the expose event, to actually do the drawing.
    g_signal_connect(G_OBJECT(window), "expose_event", G_CALLBACK(draw), pixbuf);

    //Connect to destroy signal so we can quit when the window closes.
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //Show the window
    gtk_widget_show(window);
}

int main(int argc, char **argv)
{
    unsigned int i;
    Point_t opt, xpt, ypt, zpt;
    Color_t ocol, xcol, ycol, zcol;
    Vertex_t ovtx, xvtx, yvtx, zvtx;
    Triangle_t xytri, yztri, zxtri;
    //const Triangle_t *const triangles[] = {&xytri, &yztri, &zxtri, NULL};
    const Triangle_t *triangles[25];
    Camera_t cam;
    Scene_t scene;
    TriRing12_t ring;
    Point_t ring_center;
    Vect_t ring_up, ring_first;

    /* Initialize the GTK+ and all of its supporting libraries. */
    gtk_init (&argc, &argv);
    gdk_init (&argc, &argv);

    //Set up the ring.
    Point_cfg(&ring_center, 0, 0, 0);
    Vect_cfg(&ring_first, 0, 0, 2);
    Vect_cfg(&ring_up, 0, 1, 0);
    TriRing12_cfg(&ring, &ring_center, &ring_first, &ring_up, rads(20));
    for(i=0; i<24; i++) {
        triangles[i] = &(ring.triangles[i]);
    }
    triangles[24] = NULL;

    //Setup some triangles.
    Point_cfg(&opt, 0, 0, 0);
    Point_cfg(&xpt, 1, 0, 0);
    Point_cfg(&ypt, 0, 1, 0);
    Point_cfg(&zpt, 0, 0, 1);

    Color_cfg(&ocol, 255, 255, 255);
    Color_cfg(&xcol, 255, 0, 0);
    Color_cfg(&ycol, 0, 255, 0);
    Color_cfg(&zcol, 0, 0, 255);

    Vertex_cfg(&ovtx, &opt, &ocol);
    Vertex_cfg(&xvtx, &xpt, &xcol);
    Vertex_cfg(&yvtx, &ypt, &ycol);
    Vertex_cfg(&zvtx, &zpt, &zcol);

    Triangle_cfg(&xytri, &ovtx, &xvtx, &yvtx);
    Triangle_cfg(&yztri, &ovtx, &yvtx, &zvtx);
    Triangle_cfg(&zxtri, &ovtx, &zvtx, &xvtx);


    Camera_cfg(&cam, 1.0);
    //Camera_yaw(&cam, rads(30));
    Camera_pitch(&cam, rads(20));
    //Camera_roll(&cam, rads(30));
    Camera_march(&cam, -5.0);
    
    scene.triangles = triangles;
    scene.cam = &cam;
    scene.frame_width = 1.0;
    scene.frame_height = 1.0;
    scene.img_height = 200;
    scene.img_width = 200;


    show_scene(&scene);

    /* Hand control over to the main loop. */
    gtk_main();
    return 0;
}




