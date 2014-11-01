/**
 * File: vertex.h
 *
 */
#ifndef VERTEX_H
#define VERTEX_H

#include "point.h"
#include "color.h"

/**
 * Struct: Vertex_t
 * Defines a vertex in a 3D model. A vertex has a location (<loc>, a <Point_t>)
 * and a <color> (a <Color_t>).
 */
typedef struct {
    Point_t loc;
    Color_t color;
} Vertex_t;

/**
 * Function: Vertex_cfg
 * Configures a <Vertex_t> object and returns a pointer to itself for convenience.
 *
 * The given <Point_t> and <Color_t> objects are used directly, they are not copied.
 */
Vertex_t * Vertex_cfg(Vertex_t *pThis, Point_t *loc, Color_t *color);

/**
 * Function: Vertex
 * Dynamically allocates a new <Vertex_t> object and configures it with <Vertex_cfg>.
 *
 * Aborts the program if there is not enough memory.
 */
Vertex_t * Vertex(Point_t *loc, Color_t *color);

/**
 * Function: Vertex_clone
 * Dynamically allocates a new <Vertex_t> object and configures it to match
 * the given source object. Note that this is a shallow copy, it does not clone
 * the point or color. For that use <Vertex_deepClone>.
 *
 * As with <Vertex>, aborts the program is there isn't enough memory.
 */
Vertex_t * Vertex_clone(const Vertex_t *pRhs);

/**
 * Function: Vertex_deepClone
 *
 * Does a deep copy of the given vertex object, and returns a dynamically allocated
 * object, including new dynamically allocated <Point_t> and <Color_t> objects,
 * cloned from those of the given vertex.
 */
Vertex_t * Vertex_deepClone(const Vertex_t *pRhs);

#endif
//end inclusion filter

