/**
 * File: vect.h
 *
 * Defines 3-D vectors, three-tuples of real values.
 * Structurally, these are identical to points in a 3-D space,
 * but they are conceptually distinct.
 */
#ifndef VECT_H
#define VECT_H

#include "types.h"


/**
 * Function: Vect_cfg
 * Configures the given <Vect_t> object and returns a pointer to the same object for convenience.
 */
Vect_t * Vect_cfg(Vect_t * pThis,  double x,  double y,  double z);

Vect_t * Vect_copy(Vect_t * pThis,  const Vect_t *pRhs);

/**
 * Function: Vect
 * Dynamically allocates and initializes a new <Vect_t> object with <Vect_cfg>
 *
 * Aborts the program if there is not enough memory.
 */
Vect_t * Vect(double x, double y, double z);

/**
 * Function: Vect_zero
 * Dynamically allocates a <Vect_t> with component values all set to 0.
 * Delegates to <Vect>, so all the same notes.
 */
Vect_t * Vect_zero(void);


/**
 * Function: Vect_clone
 * Dynamically allocates a new <Vect_t> object and configures it to match
 * the given source object.
 *
 * As with <Vect>, aborts the program is there isn't enough memory.
 */
Vect_t * Vect_clone(const Vect_t* pRhs);

/**
 * Function: Vect_magnitude
 *
 * Returns the magnitude of the vector.
 */
double Vect_magnitude(const Vect_t *pThis);

/**
 * Function: Vect_add
 * Adds two vectors component wise into the given output vector.
 */
Vect_t * Vect_add(Vect_t *opVect, const Vect_t *pA, const Vect_t *pB);

/**
 * Function: Vect_sub
 * Subtracts to vectors and returns their difference. Subtracts B from A, 
 * which is the same as adding negative B to A.
 */
Vect_t * Vect_sub(Vect_t *opVect, const Vect_t *pA, const Vect_t *pB);

/**
 * Function: Vect_negate
 * Produces the negative of the given vector, which is simply the vector
 * scaled by -1 (all components are additively inverted). But this is more efficient
 * than <Vect_scale>.
 */
Vect_t * Vect_negate(Vect_t *opVect, const Vect_t *pVect);

/**
 * Function: Vect_cross
 * Returns a Vect object which is the cross product of the two given vectors.
 *
 * You must provide the <opProd> output object, the same pointer is returned as
 * a convenience, no dynamic allocation is performed.
 *
 * It is acceptable for any of the given pointers to point to the same objects.
 */
Vect_t * Vect_cross(Vect_t *opProd, const Vect_t *pA, const Vect_t *pB);

/**
 * Function: Vect_dot
 * Returns the dot product of two vectors.
 */
double Vect_dot(const Vect_t *pA, const Vect_t *pB);

/**
 * Function: Vect_normalize
 * Produces a unit vector (vector of magnitude 1) pointing in the same
 * direction as the given vector.
 * Returns the pointer to the configured object as a convenience.
 *
 * The normalized vector is often denoted with a hat. It is simply the same vector
 * scaled by the multiplicative inverse of its magnitude.
 *
 * It is acceptable for <opNormal> and <pRhs> to point to the same struct.
 */
Vect_t * Vect_normalize(Vect_t *opNormal, const Vect_t *const pRhs);

/**
 * Function: Vect_scale
 * Configures the given vector to be a scaled version of the right-hand vector.
 * It is acceptable for both pointers to point to the same object.
 */
Vect_t * Vect_scale(Vect_t *pThis, const Vect_t *pRhs, double scale);

/**
 * Function: Vect_setMag
 * Sets the magnitude of the vector without changing it's direction (unless of course
 * the magnitude is negative).
 *
 * This is functionally equivalent to normalizing the vector with <Vect_normalize>
 * and then scaling it by the desired magnitude with <Vect_scale>. But it's faster to
 * do it this way.
 */
Vect_t * Vect_setMag(Vect_t *pThis, double magnitude);

/**
 * Function: Vect_angle
 * Returns the smaller of the two angles between the two vectors, in radians.
 *
 * This assumes the vectors are arranged tail to tail, and returns an angle no greater
 * than pi radians.
 */
double Vect_angle(const Vect_t *pA, const Vect_t *pB);

/**
 * Function: Vect_point
 *
 * Turns a vector into a point.
 *
 * Returns the point that this vector, situated at the origin, points to.
 */
Point_t * Vect_point(Point_t *opPoint, const Vect_t *pVect);


#endif
//end inclusion filter

