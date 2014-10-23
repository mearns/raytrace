/**
 * File: main.c
 *
 */
#include <stdio.h>

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
    double frame_width;
    double frame_height;
    int img_width;
    int img_height;
} Scene_t;

static gboolean render_scene(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    unsigned int i, j;
    const Scene_t *const scene = (const Scene_t*)(data);
    const int width = scene->img_width;
    const int height = scene->img_height;

    //Create our pix buffer.
    GdkPixbuf * pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    g_assert(gdk_pixbuf_get_n_channels(pixbuf) == 3);
    g_assert(gdk_pixbuf_get_width(pixbuf) == width);
    g_assert(gdk_pixbuf_get_height(pixbuf) == height);

    
    //Fill it in.
    const int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    guchar *const pixels = gdk_pixbuf_get_pixels(pixbuf);
    guchar *pix;
    for(i=0; i<width; i++) {
        for(j=0; j<height; j++) {
            pix = pixels + (j*rowstride) + (i*3);
            pix[0] = i % 255; //red
            pix[1] = j % 255; //green
            pix[2] = 0; //blue;
        }
    }

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

    Vertex_t *vert_a = Vertex(Point(0, 0, 5), Color(255, 0, 0));
    Vertex_t *vert_b = Vertex(Point(0, 1, 5), Color(0, 255, 0));
    Vertex_t *vert_c = Vertex(Point(1, 0, 5), Color(0, 0, 255));
    Triangle_t *triangle = Triangle(vert_a, vert_b, vert_c);

    Point_t *test_pt = Point(0.5, 0, 0);
    Color_t test_color;

    Triangle_getColor(triangle, &test_color, test_pt);

    printf("Area of triangle: %f\n", triangle->area);
    printf("Triangle at (%f, %f, %f) is [%d,%d,%d]\n", test_pt->x, test_pt->y, test_pt->z, test_color.r, test_color.g, test_color.b);

    const Triangle_t *const triangles[] = {triangle, NULL};
    Scene_t scene;
    scene.triangles = triangles;
    scene.eye = Point(0, 0, 0);
    scene.pov = Point(0, 0, 1);
    scene.frame_width = 1.0;
    scene.frame_height = 1.0;
    scene.img_height = 100;
    scene.img_width = 100;

    show_scene(&scene);

    /* Hand control over to the main loop. */
    gtk_main();
    return 0;
}




