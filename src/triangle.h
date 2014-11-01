/**
 * File: triangle.h
 *
 */
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include <stdbool.h>

#include "vertex.h"
#include "point.h"
#include "vect.h"
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
    Vertex_t vert[3];

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
    Vect_t normal;
} Triangle_t;

/**
 * Function: Triangle_barycentricPosition
 *
 * Gets the barycentric coordinates of a point relative to this triangle.
 *
 * Arguments:
 *  pThis   -   const <Triangle_t>* : The triangle.
 *  opBarry -   <Point_t>* : Pointer to the output object to hold the barycentric equations.
 *              This co-ops the X, Y, and Z coordinates of the <Point_t> struct to hold the barycentric
 *              coordinates relative to the first, second, and third vertices of the triangle,
 *              respectively.
 *  opPoint -   The point to get the barycentric coordinates of.
 */
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
 * of the colors of the triangles vertices, weighted by the barycentric coordinates of the
 * point relative to the triangle. This assumes but does not require that the point is on the
 * triangle.
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
 * Function: Triangle_rayCast
 *
 * Performs simple ray casting of a single ray onto this triangle.
 *
 * Finds the point at which a ray intersects with this triangle, and returns the distance from the start
 * of the ray to the point of intersection, as well as populating the <opColor> buffer object with 
 * the color of the triangle at the point of intersection (as with <Triangle_getColor>).
 *
 * This stops if the point of intersection with this triangle is further than the specified
 * <closest_dist>, does not modify <opColor> at all, and returns <closest_dist> back again.
 * The same applies if the ray does not intersect this triangle, including the case where
 * the presumed point of intersection would actually be _behind_ the starting point of the ray,
 * relative to the direction of the ray.
 *
 * The way to use this is to initialize a <Color_t> object as black (or whatever your scene's
 * background color is) and a variable called "closest_dist" of type double to <INFINITY>.
 * Invoke this method on each of the triangles in your scene, passing in "closest_dist"
 * and the <Color_t> buffer object, and assigning the resulting value as the new value of
 * "closest_dist". If any of the triangles intersect the ray, then the Color object will be set
 * appropriately for the closest intersection. Otherwise it will be left alone and remain the
 * background color.
 *
 * Arguments:
 *  pThis   -   const <Triangle_t>* : the triangle with which to intersect the ray.
 *  opColor -   <Color_t>* : Pointer to a color object which will be populated with the color
 *              of the triangle at the point of intersection, **if and only if** the ray intersects
 *              the triangle at a distance closer than <closest_dist>. Otherwise this object will not
 *              be touched.
 *  closest_dist    -   double : The current closest distance of intersection with other models.
 *                      If the point of intersection with this triangle is further than this, then
 *                      it will be treated as no intersection, since it is obscured by a closer model.
 *                      You should generally initialize this to <INFINITY> (from math.h).
 *  pt      -   const <Point_t>* : The starting point of the ray.
 *  vect    -   const <Vect_t>* : The vector describing the direction of the ray
 *
 */
double Triangle_rayCast(const Triangle_t *pThis, Color_t *opColor, const double closest_dist, const Point_t *const pt, const Vect_t *const vect);

/**
 * Function: Triangle_isInside
 * Checks if the given point is inside or outisde the triangle, assuming the point is actually
 * on the plane. If it is not on the plane, all bets are off.
 */
bool Triangle_isInside(const Triangle_t *const pThis, const Point_t *const pPt);




#endif
//end inclusion filter

