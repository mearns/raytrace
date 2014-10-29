/**
 * File: point.h
 *
 */
#ifndef POINT_H
#define POINT_H

#include "types.h"

/**
 * Function: Point_cfg
 * Configures the given <Point_t> object and returns a pointer to the same object for convenience.
 */
Point_t * Point_cfg(Point_t * pThis,  double x,  double y,  double z);

/**
 * Function: Point_copy
 * 
 * Configured the given <Point_t> structure to be a copy of the given point.
 */
Point_t * Point_copy(Point_t * pThis,  const Point_t *pRhs);

/**
 * Function: Point
 * Dynamically allocates and initializes a new <Point_t> object with <Point_cfg>
 *
 * Aborts the program if there is not enough memory.
 */
Point_t * Point(double x, double y, double z);

/**
 * Function: Point_clone
 * Dynamically allocates a new <Point_t> object and configures it to match
 * the given source object.
 *
 * As with <Point>, aborts the program is there isn't enough memory.
 */
Point_t * Point_clone(const Point_t* pRhs);

/**
 * Function: Point_zero
 * Dynamically allocates a <Point_t> with coordinate values all set to 0.
 * Delegates to <Point>, so all the same notes.
 */
Point_t * Point_zero(void);

/**
 * Function: Point_distance
 *
 * Returns the distance between two points.
 */
double Point_distance(const Point_t *pThis, const Point_t *pOther);

/**
 * Function: Point_translate
 * Translates a point by a vector. This is simply component-wise addition of the
 * points components and the vectors elements. Populates the given output point
 * with the resulting translated point.
 *
 * It is acceptable for the input and output <Point_t> pointers to point to the same
 * object.
 */
Point_t * Point_translate(Point_t *opPoint, const Point_t *pPt, const Vect_t *pTrans);

/**
 * Function: Point_translateBack
 * Like <Point_translate>, but translates by the negative of the given vector.
 */
Point_t * Point_translateBack(Point_t *opPoint, const Point_t *pPt, const Vect_t *pTrans);

/**
 * Function: Point_displacement
 * Get the displacement from point A to point B, and returns the displacement vector
 * as a <Vect_t> object. This **does not do dynamic allocation**, you have to supply
 * the <opDisp> parameter as an object to store the result in. The same point is
 * returned as a convenience.
 *
 */
Vect_t * Point_displacement(Vect_t *opDisp, const Point_t *pA, const Point_t *pB);

/**
 * Function: Point_position
 *
 * Turns a point into a vector. 
 *
 * Gets the _position vector_ of the given point, which is the vector pointing
 * from the origin, (0,0,0), to the point. This is a vector whose elements are
 * equal to the corresponding components of the point.
 */
Vect_t * Point_position(Vect_t *opVect, const Point_t *pPt);

#endif
//end inclusion filter

