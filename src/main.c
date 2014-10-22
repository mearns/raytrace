/**
 * File: main.c
 *
 */
#include <stdio.h>

#include "triangle.h"
#include "point.h"
#include "vertex.h"
#include "color.h"


int main(void) {

    Color_t col_a = {.r=255, .g=0, .b=0};
    Color_t col_b = {.r=0, .g=255, .b=0};
    Color_t col_c = {.r=0, .g=0, .b=255};

    Point_t point_a = {.x=0.0, .y=0.0, .z=0.0};
    Point_t point_b = {.x=0.0, .y=1.0, .z=0.0};
    Point_t point_c = {.x=1.0, .y=0.0, .z=0.0};

    Vertex_t vert_a = {.loc = &point_a, .color = &col_a};
    Vertex_t vert_b = {.loc = &point_b, .color = &col_b};
    Vertex_t vert_c = {.loc = &point_c, .color = &col_c};

    Triangle_t triangle;
    triangle.vert[0] = &vert_a;
    triangle.vert[1] = &vert_b;
    triangle.vert[2] = &vert_c;

    Point_t test_pt = {.x=0, .y=0, .z=0};
    Color_t test_color;

    Triangle_getColor(&triangle, &test_color, &test_pt);

    printf("Triangle at (%f, %f, %f) is [%d,%d,%d]\n", test_pt.x, test_pt.y, test_pt.z, test_color.r, test_color.g, test_color.b);

    return 0;
}




