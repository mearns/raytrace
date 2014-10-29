/**
 * File: plane.h
 * Defines a plane in 3 dimensions.
 */
#ifndef PLAIN_H
#define PLAIN_H

#include "point.h"
#include "vect.h"

/**
 *
 * Struct: Plane_t
 *
 * Describes a plane in three dimensions. The equation of a plane is
 *      $$ ax + by + cz + d = 0 $$
 * 
 * Where $ <a, b, c> $ is a vector normal to the plane.
 */
typedef struct {
    double params[4];
} Plane_t;

/**
 * Function: Plane
 * Dynamically allocates a new <Plane_t> object and configures it with <Plane_cfg>.
 *
 * Aborts program if not enough memory.
 */
Plane_t * Plane(double a, double b, double c, double d);

/**
 * Function: Plane
 * Clones the given plane.
 *
 * Like <Plane>, aborts the program if not enough memory.
 */
Plane_t * Plane_clone(const Plane_t *pRhs);

/**
 * Function: Plane_cfg
 * Configures the Plane object with the given parameters, where the plane is 
 * defined by the equation:
 *
 *      $$ ax + by + cz + d = 0 $$
 */
Plane_t * Plane_cfg(Plane_t *pThis, double a, double b, double c, double d);

Plane_t * Plane_copy(Plane_t *pThis, const Plane_t *pRhs);

/**
 * Function: Plane_cfgFromThreePoints
 * Configures the Plane object to define a plane that passes through the three given points.
 */
Plane_t * Plane_cfgFromThreePoints(Plane_t *pThis, const Point_t *pt1, const Point_t *pt2, const Point_t *pt3);

/**
 * Function: Plane_getPoint
 * Returns an arbitrary point on the plane.
 */
Point_t * Plane_getPoint(const Plane_t *const pThis, Point_t *opPoint);

/**
 * Function: Plane_getNormal
 * Gets a vector normal to the plane. Specifically, this is the vector <a,b,c>,
 * where the plane is defined by the parmaters a, b, c, and d.
 */
Vect_t * Plane_getNormal(const Plane_t *pThis, Vect_t *opNormal);

#endif
//end inclusion filter

