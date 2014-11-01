/**
 * File: axes.h
 *
 * Represents a local 3-D coordinate system. For instance, you can use coordinates
 * in the local coordinate system to keep things simple, but that coordinate system
 * could be scaled, rotated, and/or translated relative to the parent coordinate
 * system.
 */
#ifndef AXES_H
#define AXES_H

#include "vect.h"
#include "point.h"
#include "quat.h"

typedef struct {
    Vect_t x;
    Vect_t y;
    Vect_t z;
    Point_t origin;
} Axes_t;

Axes_t * Axes_cfg(Axes_t *pThis);

Axes_t * Axes(void);

Axes_t * Axes_copy(Axes_t *pThis, const Axes_t *const pRhs);

Axes_t * Axes_clone(const Axes_t *const pRhs);

//TODO: Add Axes_rotate, to rotate around arbitrary axes. Probably a local vector?

/**
 * Function: Axes_yaw
 * Rotates the axes around it's Y-axis.
 */
Axes_t * Axes_yaw(Axes_t *const pThis, const double rads);

/**
 * Function: Axes_pitch
 * Rotates the axes around it's X-axis.
 */
Axes_t * Axes_pitch(Axes_t *const pThis, const double rads);

/**
 * Function: Axes_roll
 * Rotates the axes around it's Z-axis.
 */
Axes_t * Axes_roll(Axes_t *const pThis, const double rads);


/**
 * Function: Axes_march
 * Translates the axes along it's Z-axis.
 */
Axes_t * Axes_march(Axes_t *const pThis, const double dist);

/**
 * Function: Axes_strafe
 * Translates the axes along it's X-axis.
 */
Axes_t * Axes_strafe(Axes_t *const pThis, const double dist);

/**
 * Function: Axes_climb
 * Translates the axes along it's Y-axis.
 */
Axes_t * Axes_climb(Axes_t *const pThis, const double dist);


/**
 * Function: Axes_point
 *
 * Translates a point in the local coordinate system to a global coordinate system.
 * 
 * Arguments:
 *  pThis - const <Axes_t>* : The axes on which this method is invoked.
 *
 *  opGlobal - <Point_t>* : The outut buffer object which will receive the global
 *      coordinates of the given point.
 *
 *  pLocal - const <Point_t>* : The point in the local coordinate system.
 * 
 * Returns:
 *  Returns the <opGlobal> pointer for convenience.
 * 
 */
Point_t * Axes_point(const Axes_t *const pThis, Point_t *const opGlobal, const Point_t *const pLocal);

/**
 * Function: Axes_vect
 *
 * Translates a vector in the local coordinate system to a global coordinate system.
 * 
 * Arguments:
 *  pThis - const <Axes_t>* : The axes on which this method is invoked.
 *
 *  opGlobal - <Vect_t>* : The outut buffer object which will receive the global
 *      coordinates of the vector.
 *
 *  pLocal - const <Vect_t>* : The vector in the local coordinate system.
 * 
 * Returns:
 *  Returns the <opGlobal> pointer for convenience.
 * 
 */
Vect_t * Axes_vect(const Axes_t *const pThis, Vect_t *const opGlobal, const Vect_t *const pLocal);

#endif
//end inclusion filter


