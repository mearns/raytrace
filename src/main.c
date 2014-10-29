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

typedef struct {
    const Triangle_t *const * triangles;
    const Point_t *eye;
    const Vect_t *pov;
    const Vect_t *up;
    double frame_width;
    double frame_height;
    int img_width;
    int img_height;
} Scene_t;

typedef struct {
    Point_t top_left;
    Vect_t step_down;
    Vect_t step_right;
} Frame_t;

static void Frame_cfg(Frame_t *pThis, const Scene_t *const scene)
{
    Vect_t up;
    Vect_t right;

    //Create the vector pointing right as the cross product of up and pov.
    Vect_cross(&right, scene->pov, scene->up);
    
    //Now make sure up is really up, i.e., perpendicular to both pov and right
    // (because right is a cross product of pov, we know right is perpendicular to pov.
    // The cross product of right and pov will be perp to both, so all three will be
    // mutually perpendicular.
    Vect_cross(&up, &right, scene->pov);

    //Make up be half the height of the frame, and right be half the width.
    Vect_setMag(&up, (scene->frame_height)*0.5);
    Vect_setMag(&right, (scene->frame_width)*0.5);

    //Get a point in the top-left corner of the frame by starting at the eye,
    // translating to the center of the frame with pov, then translating to the top-center
    // of the frame with up, and then translating back to the top-left corner with right.
    Point_translate(&(pThis->top_left), scene->eye, scene->pov);
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
    double distance, min_dist;
    Color_t render_color, test_color;
    const Triangle_t *const *pTriangle;
    guchar *scanline;
    guchar *pix;
    double frame_dist;

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
            Point_displacement(&ray, scene->eye, &pt);

            //And the distance from the eye to the frame, so we can make sure not to render
            // anything on the wrong side of the frame.
            frame_dist = Vect_magnitude(&ray);

            //Find which triangle it intersect withs closest.
            min_dist = INFINITY;
            Color_cfg(&render_color, 0, 0, 0);
            pTriangle = scene->triangles;
            for(pTriangle = scene->triangles; *(pTriangle) != NULL; pTriangle++)
            {
                distance = Triangle_intersect(*pTriangle, &test_color, scene->eye, &ray);

                if (distance >= frame_dist && distance < min_dist) {
                    Color_copy(&render_color, &test_color);
                    min_dist = distance;
                }
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
    const Scene_t *const scene = (const Scene_t*)(data);
    const int width = scene->img_width;
    const int height = scene->img_height;

    //Create our pix buffer.
    GdkPixbuf * pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    g_assert(gdk_pixbuf_get_n_channels(pixbuf) == 3);
    g_assert(gdk_pixbuf_get_width(pixbuf) == width);
    g_assert(gdk_pixbuf_get_height(pixbuf) == height);

    //Draw on in.
    render_scene(pixbuf, scene);
    

    //Draw it to the window.
    gdk_draw_pixbuf(widget->window, NULL, pixbuf, 0, 0, 0, 0, width, height, GDK_RGB_DITHER_NONE, 0, 0);

    return TRUE;
}

//TODO: Would love to allow a const Scene_t here, so scenes can be built at compile time and left in ROM.
// but the g_signal_connect complains about passing const, and I don't like casting const to non-const.
void show_scene(Scene_t *const scene)
{
    GtkWidget *window;
    
    //Creat the GTK window.
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), scene->img_width, scene->img_height);
    gtk_window_set_title (GTK_WINDOW (window), "Ray Trace");

    //Connect it to the expose event, to actually do the drawing.
    //TODO: Can probably create and draw the pixbuf once, just need to draw it to the window in the callback.
    g_signal_connect(G_OBJECT(window), "expose_event", G_CALLBACK(draw), scene);

    //Connect to destroy signal so we can quit when the window closes.
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //Show the window
    gtk_widget_show(window);
}

int main(int argc, char **argv)
{
    /* Initialize the GTK+ and all of its supporting libraries. */
    gtk_init (&argc, &argv);
    gdk_init (&argc, &argv);

    //TODO: This is very bad, lots of dynamic allocation and no freeing!
    // good thing it's main.

    Vertex_t *vert_a = Vertex(Point(0, 0, 5), Color(255, 0, 0));
    Vertex_t *vert_b = Vertex(Point(0, 1, 5), Color(0, 255, 0));
    Vertex_t *vert_c = Vertex(Point(1, 0, 5), Color(0, 0, 255));
    Triangle_t *triangle = Triangle(vert_a, vert_b, vert_c);

    const Triangle_t *const triangles[] = {triangle, NULL};
    Scene_t scene;
    scene.triangles = triangles;
    scene.eye = Point(0, 0, 0);
    scene.pov = Vect(0, 0, 1);
    scene.up = Vect(0, 1, 0);
    scene.frame_width = 1.0;
    scene.frame_height = 1.0;
    scene.img_height = 200;
    scene.img_width = 200;

    show_scene(&scene);

    /* Hand control over to the main loop. */
    gtk_main();
    return 0;
}




