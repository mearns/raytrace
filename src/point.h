/**
 * File: point.h
 *
 */
#ifndef POINT_H
#define POINT_H

/**
 * Struct: Point_t
 *
 * Defines a point in 3-D cartesian space by X, Y, and Z coordinates.
 * Can also represent a vector in three dimensions.
 */
typedef struct {
    double x;
    double y;
    double z;
} Point_t;


/**
 * Function: Point_cfg
 * Configures the given <Point_t> object and returns a pointer to the same object for convenience.
 */
Point_t * Point_cfg(Point_t * pThis,  double x,  double y,  double z);

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
 * Function: Point_distance
 *
 * Returns the distance between two points.
 */
double Point_distance(const Point_t *pThis, const Point_t *pOther);

Point_t * Point_add(Point_t *opPoint, const Point_t *pA, const Point_t *pB);

/**
 * Function: Point_sub
 * Subtracts to vectors and returns their difference. Subtracts B from A, 
 * which is the same as adding negative B to A.
 *
 * This is just a small wrapper around <Point_displacement>, switching the order
 * of the args.
 */
#define Point_sub(oPOINT, pA, pB)   \
    Point_displacement((oPOINT), (pB), (pA))

/**
 * Function: Point_displacement
 * Get the displacement from point A to point B, and returns the displacement vector
 * as a <Point_t> object. This **does not do dynamic allocation**, you have to supply
 * the <opDisp> parameter as an object to store the result in. The same point is
 * returned as a convenience.
 *
 */
Point_t * Point_displacement(Point_t *opDisp, const Point_t *pA, const Point_t *pB);

/**
 * Function: Point_crossProduct
 * Interpretting the two points, A and B, as vectors, returns another Point object
 * which is the vector representing their cross product.
 *
 * You must provide the <opProd> output object, the same pointer is returned as
 * a convenience, no dynamic allocation is performed.
 */
Point_t * Point_crossProduct(Point_t *opProd, const Point_t *pA, const Point_t *pB);

/**
 * Function: Point_dotProduct
 * Returns the dot product of two points.
 */
double Point_dotProduct(const Point_t *pA, const Point_t *pB);

/**
 * Function: Point_normalize
 * Configures the given point to be the unit vector of the given point.
 * Returns the pointer to the configured object as a convenience.
 *
 * It is acceptable for <opNormal> and <pRhs> to point to the same struct.
 */
Point_t * Point_normalize(Point_t *opNormal, const Point_t *const pRhs);

/**
 * Function: Point_length
 * Returns the length of the vector represented by the given Point.
 */
double Point_length(const Point_t *pThis);

/**
 * Function: Point_scale
 * Configures the given point to be a scaled version of the right-hand point.
 * It is acceptable for both points to be the same.
 */
Point_t * Point_scale(Point_t *pThis, const Point_t *pRhs, double scale);

/**
 * Function: Point_vectorAngle
 * Returns the smaller of the two angles between the two vectors, in radians.
 */
double Point_vectorAngle(const Point_t *pA, const Point_t *pB);

#endif
//end inclusion filter

