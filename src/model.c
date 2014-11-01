/**
 * File: model.c
 *
 */
#include "model.h"

#include <stdlib.h>

#include "axes.h"
#include "triangle.h"


Model_t * Model_cfg(Model_t *const pThis)
{
    Axes_cfg(&(pThis->axes));
    pThis->children = NULL;
    pThis->triangles = NULL;

    return pThis;
}

Model_t * Model_yaw(Model_t *const pThis, const double rads)
{
    Axes_yaw(&(pThis->axes), rads);
    return pThis;
}

Model_t * Model_pitch(Model_t *const pThis, const double rads)
{
    Axes_pitch(&(pThis->axes), rads);
    return pThis;
}

Model_t * Model_roll(Model_t *const pThis, const double rads)
{
    Axes_roll(&(pThis->axes), rads);
    return pThis;
}

Model_t * Model_march(Model_t *const pThis, const double dist)
{
    Axes_march(&(pThis->axes), dist);
    return pThis;
}

Model_t * Model_strafe(Model_t *const pThis, const double dist)
{
    Axes_strafe(&(pThis->axes), dist);
    return pThis;
}

Model_t * Model_climb(Model_t *const pThis, const double dist)
{
    Axes_climb(&(pThis->axes), dist);
    return pThis;
}



