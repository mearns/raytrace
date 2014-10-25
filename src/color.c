/**
 * File: color.c
 *
 */
#include "color.h"

#include <stdlib.h>

#include "util.h"

Color_t * Color(const uint8_t r, const uint8_t g, const uint8_t b)
{
    Color_t *const pThis = Util_allocOrDie(sizeof(Color_t), "Allocating a Color_t object.");
    if(pThis != NULL) {
        Color_cfg(pThis, r, g, b);
    }
    return pThis;
}

Color_t * Color_cfg(Color_t *const pThis, const uint8_t r, const uint8_t g, const uint8_t b)
{
    pThis->r = r;
    pThis->g = g;
    pThis->b = b;
    return pThis;
}

Color_t * Color_copy(Color_t *const pThis, const Color_t *const pRhs)
{
    return Color_cfg(pThis, pRhs->r, pRhs->g, pRhs->b);
}

Color_t * Color_clone(const Color_t *pRhs)
{
    return Util_cloneOrDie(pRhs, sizeof(Color_t), "Cloning a Color_t object.");
}

