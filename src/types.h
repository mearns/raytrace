/**
 * File: types.h
 *
 */
#ifndef TYPES_H
#define TYPES_H


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
 * Struct: Vect_t
 *
 * Defines a vector in 3-D cartesian space by X, Y, and Z real components.
 */
typedef struct {
    double x;
    double y;
    double z;
} Vect_t;

#endif
//end inclusion filter

