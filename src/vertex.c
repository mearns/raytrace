/**
 * File: vertex.c
 *
 */
#include "vertex.h"

#include "point.h"
#include "color.h"
#include "util.h"

Vertex_t * Vertex_cfg(Vertex_t *pThis, Point_t *loc, Color_t *color)
{
    Point_copy(&(pThis->loc), loc);
    Color_copy(&(pThis->color), color);
    return pThis;
}

Vertex_t * Vertex(Point_t *loc, Color_t *color)
{
    Vertex_t *const pThis = Util_allocOrDie(sizeof(Vertex_t), "Allocating a Vertex_t object.");
    if(pThis != NULL) {
        Vertex_cfg(pThis, loc, color);
    }
    return pThis;
}

Vertex_t * Vertex_clone(const Vertex_t *pRhs)
{
    return Util_cloneOrDie(pRhs, sizeof(Vertex_t), "Cloning a Vertex_t object.");
}

Vertex_t * Vertex_deepClone(const Vertex_t *pRhs)
{
    return Vertex(Point_clone(&(pRhs->loc)), Color_clone(&(pRhs->color)));
}



