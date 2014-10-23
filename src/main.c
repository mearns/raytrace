/**
 * File: main.c
 *
 */
#include <stdio.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "triangle.h"
#include "point.h"
#include "vertex.h"
#include "color.h"


int main(int argc, char **argv)
{

    GtkWidget *window;

    /* Initialize the GTK+ and all of its supporting libraries. */
    gtk_init (&argc, &argv);
    gdk_init (&argc, &argv);

    /* Create a new window, give it a title and display it to the user. */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
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

    /* Hand control over to the main loop. */
    gtk_main ();
    return 0;
}




