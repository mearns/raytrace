/**
 * File: quats.h
 *
 */
#ifndef QUATS_H
#define QuATS_H

#include "vect.h"
#include "point.h"

typedef struct {
    double w;
    double x;
    double y;
    double z;
} Quat_t;

Quat_t * Quat_cfg(Quat_t *pThis, double w, double x, double y, double z);

Quat_t * Quat_copy(Quat_t *pThis, const Quat_t *pRhs);

Quat_t * Quat(double w, double w, double y, double z);

Quat_t * Quat_clone(const Quat_t *pRhs);

/**
 * Function: Quat_rotation
 * Generates a quaternion representing rotation by a specified angle around a specified axis.
 *
 * Arguments:
 *  pThis   -   <Quat_t>* : The output structure to hold the quaternion that represents the rotation.
 *  pAxis   -   const <Vect_t>* : The vector representing the axis of rotation.
 *  rads    -   double : The angle of rotation, in radians.
 */
Quat_t * Quat_rotation(Quat_t *pThis, const Vect_t *pAxis, double rads);

/**
 * Function: Quat_product
 * Does the Hamilton product of two quats, pA times pB. Populates the output structure
 * with the resulting product quat.
 */
Quat_t * Quat_product(Quat_t *opThis, const Quat_t *pA, const Quat_t *pB);

/**
 * Function: Quat_conjugate
 * Returns the quaternion conjugate of the given quat.
 *
 * This simply has the x, y, and z components negated, the real component (w)
 * stays the same.
 */
Quat_t * Quat_conjugate(Quat_t *opThis, const Quat_t *pRhs);

/**
 * Function: Quat_rotatePoint
 *
 * Rotates the given point by the three-dimensional rotation represented by this quat.
 * Note that only unit quaternions are valid three-dimensional rotations, using a non-unit
 * quat will have unspecified results.
 *
 */
Point_t * Quat_rotatePoint(const Quat_t *pThis, Point_t *opRotated, const Point_t *pPoint);

/**
 * Function: Quat_rotateVect
 * Rotates a vector by the rotation represented by this quat.
 *
 * This is the same as <Quat_rotatePoint>, except it operates on vectors instead of points.
 */
Vect_t * Quat_rotateVect(const Quat_t *pThis, Vect_t *opRotated, const Vect_t *pVect);


#endif
//end inclusion filter

