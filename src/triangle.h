/**
 * File: triangle.h
 *
 */
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

#include "vertex.h"
#include "point.h"
#include "color.h"
#include "plane.h"

typedef enum {VTX1=0, VTX2=1, VTX3=2} TriangleVertIndex_t;

/**
 * Struct: Triangle_t
 * A primitive element of a model, a triangle is defined by three vertices, specified
 * in its <vert> property (each is a <Vertex_t> object).
 *
 * Color at any given point on either side of the triangle is the weighted average of
 * the colors of its vertices, weighted by the barrycentric coordinates of the point.
 */
typedef struct {
    Vertex_t *vert[3];

    /**
     * Field: area
     * The signed area of the triangle.
     */
    double area;

    /**
     * Field: normal
     * A unit vector normal to the triangle. This is the cross product of the vector from VTX1 to VTX2
     * and the vector from VTX1 to VTX3.
     */
    Point_t normal;
} Triangle_t;

Point_t * Triangle_barycentricPosition(const Triangle_t *pThis, Point_t *opBarry, const Point_t *opPoint);

/**
 * Function: Triangle_cfg
 * Configures a <Triangle_t> object and returns a pointer to itself for convenience.
 *
 * The given <Vertex_t> objects are used directly, they are not copied.
 */
Triangle_t* Triangle_cfg(Triangle_t *pThis, Vertex_t *pVertex1, Vertex_t *pVertex2, Vertex_t *pVertex3);

/**
 * Function: Triangle
 * Dynamically allocates a new <Triangle_t> object and configures it with <Triangle_cfg>.
 *
 * Aborts the program if there is not enough memory.
 */
Triangle_t* Triangle(Vertex_t *pVertex1, Vertex_t *pVertex2, Vertex_t *pVertex3);

/**
 * Function: Triangle_clone
 * Dynamically allocates a new <Triangle_t> object and configures it to match
 * the given source object.
 *
 * As with <Triangle>, aborts the program is there isn't enough memory.
 */
Triangle_t* Triangle_clone(const Triangle_t *pRhs);

/**
 * Function: Triangle_getColor
 * Determines the color of the triangle at the specified point. The color is simply the average
 * of the colors of the triangles vertices, weighted by distance to the vertex. This assumes but
 * does not require that the point is on the triangle.
 *
 * The resulting color is placed in <opColor>.
 *
 * Arguments:
 *  pThis   -   const <Triangle_t>* : The triangle on which the method is invoked.
 *  opColor -   <Color_t>*: Pointer to the color object to which the result will be stored.
 *  pPt     -   const <Point_t>* : The point, presumably on the triangle, to get the color of.
 */
Color_t * Triangle_getColor(const Triangle_t *pThis, Color_t *opColor, const Point_t *pPt);

/**
 * Function: Triangle_intersect
 *
 * Finds the point at which a ray intersects with this triangle, and returns the distance from the start
 * of the ray to the point of intersection, as well as populating the <opColor> buffer object with 
 * the color of the triangle at the point of intersection (as with <Triangle_getColor>).
 *
 * Returns <INFINITY> if there is no intersection between the ray and the triangle, which includes if the
 * ray is parallel to the triangle, if the ray intersects the plane of the tiangle but not the triangle itself,
 * or if the triangle is "behind" the ray.
 *
 * Arguments:
 *  pThis   -   const <Triangle_t>* : the triangle with which to intersect the ray.
 *  opColor -   <Color_t>* : Pointer to a color object which will be populated with the color
 *              of the triangle at the point of intersection, **if and only if* the ray intersects
 *              the triangle. Otherwise this object will not be touched.
 *  pt      -   const <Point_t>* : The starting point of the ray.
 *  vect    -   const <Point_t>* : The vector describing the direction of the ray
 *
 * Returns:
 *  A double indicating the distance from <pt> (the start of the ray) to the point of intersection,
 *  or <INFINITY> (from "math.h") if the ray does not intersect the triangle.
 */
double Triangle_intersect(const Triangle_t *pThis, Color_t *opColor, const Point_t *const pt, const Point_t *const vect);

#endif
//end inclusion filter




