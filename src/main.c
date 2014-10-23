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

static const int WIDTH = 200;
static const int HEIGHT = 200;

gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    unsigned int i, j;

    GdkPixbuf * pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, WIDTH, HEIGHT);
    g_assert(gdk_pixbuf_get_n_channels(pixbuf) == 3);
    g_assert(gdk_pixbuf_get_width(pixbuf) == WIDTH);
    g_assert(gdk_pixbuf_get_height(pixbuf) == HEIGHT);

    
    const int rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    guchar *const pixels = gdk_pixbuf_get_pixels (pixbuf);
    guchar *pix;
    for(i=0; i<WIDTH; i++) {
        for(j=0; j<HEIGHT; j++) {
            pix = pixels + (j*rowstride) + (i*3);
            pix[0] = i % 255; //red
            pix[1] = j % 255; //green
            pix[2] = 0; //blue;
        }
    }

    gdk_draw_pixbuf(widget->window, NULL, pixbuf, 0, 0, 0, 0, WIDTH, HEIGHT, GDK_RGB_DITHER_NONE, 0, 0);

    return TRUE;
}

int main(int argc, char **argv)
{

    GtkWidget *window;

    /* Initialize the GTK+ and all of its supporting libraries. */
    gtk_init (&argc, &argv);
    gdk_init (&argc, &argv);

    /* Create a new window, give it a title and display it to the user. */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
    gtk_window_set_title (GTK_WINDOW (window), "Hello World");
    gtk_widget_show (window);


    Vertex_t *vert_a = Vertex(Point(0, 0, 0), Color(255, 0, 0));
    Vertex_t *vert_b = Vertex(Point(0, 1, 0), Color(0, 255, 0));
    Vertex_t *vert_c = Vertex(Point(1, 0, 0), Color(0, 0, 255));
    Triangle_t *triangle = Triangle(vert_a, vert_b, vert_c);

    Point_t *test_pt = Point(0.5, 0, 0);
    Color_t test_color;

    Triangle_getColor(triangle, &test_color, test_pt);

    printf("Area of triangle: %f\n", triangle->area);
    printf("Triangle at (%f, %f, %f) is [%d,%d,%d]\n", test_pt->x, test_pt->y, test_pt->z, test_color.r, test_color.g, test_color.b);

    g_signal_connect(G_OBJECT(window), "expose_event", G_CALLBACK(expose_event_callback), NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Hand control over to the main loop. */
    gtk_main ();
    return 0;
}




