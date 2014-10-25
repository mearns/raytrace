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
#include "vertex.h"
#include "color.h"

typedef struct {
    const Triangle_t *const * triangles;
    const Point_t *eye;
    const Point_t *pov;
    const Point_t *up;
    double frame_width;
    double frame_height;
    int img_width;
    int img_height;
} Scene_t;

/**
 *
 * Coordinates:
 *
 * Note that this is a right-handed coordinate system, which may be counterintuitive in some cases.
 * X is left and right, Y is up and down, Z is in and out (of the screen), but positive X
 * is **to the left**, positive Y is up, and positive Z is into the screen.
 */
static gboolean render_scene(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    unsigned int i, j;
    const Scene_t *const scene = (const Scene_t*)(data);
    const int width = scene->img_width;
    const int height = scene->img_height;
    const Point_t *const eye = scene->eye;
    const Point_t *const pov = scene->pov;

    //TODO: Dynamic allocation was for convenience, but all of these should be statically allocated on the stack.

    //Up is a vector pointing from the center of the frame to the top of the frame.
    Point_t *const up = Point_clone(scene->up);
    
    //And right is a vector from the center of the frame to the right edge.
    Point_t *const right = Point_crossProduct(Point(0,0,0), pov, up);
    Point_normalize(right, right);
    Point_scale(right, right, scene->frame_width/2.0);

    // Make sure up is really up, i.e., really perpindicular to pov (and right).
    Point_crossProduct(up, right, pov);
    Point_normalize(up, up);
    Point_scale(up, up, scene->frame_height/2.0);

    //This is a point in the top-left corner of the frame.
    Point_t *const top_left = Point_add(Point(0,0,0), eye, pov);    //To center of frame.
    Point_add(top_left, top_left, up);    //To top of frame.
    Point_sub(top_left, top_left, right);   //To top-left corner.

    //Step by one pixel in each direction.
    Point_t *const step_right = Point_scale(Point(0,0,0), right, 1.0 / ((double)(width) * 0.5));
    Point_t *const step_down = Point_scale(Point(0,0,0), up, -1.0 / ((double)(height) * 0.5));

    //The point we cast rays through.
    Point_t pt;
    Point_t ray;
    Point_t row_start;

    //Create our pix buffer.
    GdkPixbuf * pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    g_assert(gdk_pixbuf_get_n_channels(pixbuf) == 3);
    g_assert(gdk_pixbuf_get_width(pixbuf) == width);
    g_assert(gdk_pixbuf_get_height(pixbuf) == height);

    
    //Fill it in.
    const int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    guchar *const pixels = gdk_pixbuf_get_pixels(pixbuf);
    guchar *pix;

    double distance, min_dist;
    Color_t render_color, test_color;
    const Triangle_t *const *pTriangle;

    Point_copy(&row_start, top_left);
    for(j=0; j<height; j++) {
        Point_copy(&pt, &row_start);
        //puts("-----");
        for(i=0; i<width; i++) {
            Point_displacement(&ray, eye, &pt);

            //Find which triangle it intersect withs closest.
            min_dist = INFINITY;
            Color_cfg(&render_color, 0, 0, 0);
            pTriangle = scene->triangles;
            for(pTriangle = scene->triangles; *(pTriangle) != NULL; pTriangle++)
            {
                distance = Triangle_intersect(*pTriangle, &test_color, eye, &ray);

                //TODO: Should be past the frame
                if (distance >= 0 && distance < min_dist) {
                    Color_copy(&render_color, &test_color);
                    min_dist = distance;
                }
            }

            pix = pixels + (j*rowstride) + (i*3);
            pix[0] = render_color.r;
            pix[1] = render_color.g;
            pix[2] = render_color.b;

            //Step to the next pixel.
            Point_add(&pt, &pt, step_right);
        }

        //Step to the next row.
        Point_add(&row_start, &row_start, step_down);
    }

    //Draw it to the window.
    gdk_draw_pixbuf(widget->window, NULL, pixbuf, 0, 0, 0, 0, width, height, GDK_RGB_DITHER_NONE, 0, 0);

    free(up);
    free(right);
    free(top_left);
    free(step_right);
    free(step_down);

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
    g_signal_connect(G_OBJECT(window), "expose_event", G_CALLBACK(render_scene), scene);

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
    Vertex_t *vert_b = Vertex(Point(0, 1, 5), Color(255, 0, 0));
    Vertex_t *vert_c = Vertex(Point(1, 0, 5), Color(0, 0, 255));
    Triangle_t *triangle = Triangle(vert_a, vert_b, vert_c);

    Vertex_t *vert_d = Vertex(Point(1, 1, 5), Color(0, 0, 255));
    Triangle_t *tri2 = Triangle(vert_b, vert_c, vert_d);

    const Triangle_t *const triangles[] = {triangle, tri2, NULL};
    Scene_t scene;
    scene.triangles = triangles;
    scene.eye = Point(0, 0, 0);
    scene.pov = Point(0, 0, 1);
    scene.up = Point(0, 1, 0);
    scene.frame_width = 1.0;
    scene.frame_height = 1.0;
    scene.img_height = 200;
    scene.img_width = 200;

    show_scene(&scene);

    /* Hand control over to the main loop. */
    gtk_main();
    return 0;
}




