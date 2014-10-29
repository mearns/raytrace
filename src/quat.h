/**
 * File: quat.h
 *
 * About Quaternions:
 *
 * Quaternions are a four-dimensional extension to the two-dimensional complex numbers.
 * As a complex number is a vector with basis (1, i), a quat is a vector with basis
 * (1, i, j, k), where i, j, and k are all imaginary (non-real) numbers.
 *
 * While the complex numbers are defined by the identity i^2 = 1, the quaternions are
 * defined by a slightly larger set of axiomatic identities:
 *
 * > i^2 = j^2 = k^2 = ijk = -1
 *
 * From these equations, we can derive all other possible products of the basis vectors.
 * Importantly, quaternion mltiplication is not commutative so, in general, QP != PQ
 * for quaternions Q and P. Using this and the equations above, we can find (through
 * left- or right- multiplication and cancelation):
 *
 * > ij = k
 * > ji = -k
 * > jk = i
 * > kj = -i
 * > ki = j
 * > ik = -j
 *
 * Note that -1Q = Q(-1) = -Q. In other words, multiplication by a scalar (real value)
 * is commutative.
 *
 *
 * Quats as Rotation:
 *
 * This is not worth getting into details over here, but quats can be used to represent
 * any rotation in three-dimensional space. That's why we are using them in this project.
 *
 */
#ifndef QUATS_H
#define QUATS_H

#include "vect.h"
#include "point.h"
#include "vertex.h"
#include "triangle.h"

typedef struct {
    double w;
    double x;
    double y;
    double z;
} Quat_t;

Quat_t * Quat_cfg(Quat_t *pThis, double w, double x, double y, double z);

Quat_t * Quat_copy(Quat_t *pThis, const Quat_t *pRhs);

Quat_t * Quat(double w, double x, double y, double z);

Quat_t * Quat_clone(const Quat_t *pRhs);

Quat_t * Quat_zero(void);

/**
 * Function: Quat_rotation
 * Generates a quaternion representing rotation by a specified angle around a specified axis.
 *
 * Angle is measured in radians clockwise around the axis when looking along the same direction
 * as the axis vector.
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

Vertex_t * Quat_rotateVertexInPlace(const Quat_t *pThis, Vertex_t *pVertex);

Triangle_t * Quat_rotateTriangleInPlace(const Quat_t *pThis, Triangle_t *pTriangle);

#endif
//end inclusion filter

