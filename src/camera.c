/**
 * File: camera.c
 *
 */
#include "camera.h"

#include "axes.h"

Camera_t * Camera_cfg(Camera_t *const pThis, double frame_dist)
{
    Axes_cfg(&(pThis->axes));
    pThis->frame_dist = frame_dist;

    return pThis;
}

Camera_t * Camera_copy(Camera_t *const pThis, const Camera_t *const pRhs)
{
    Axes_copy(&(pThis->axes), &(pRhs->axes));
    pThis->frame_dist = pRhs->frame_dist;

    return pThis;
}

Point_t * Camera_getEye(const Camera_t *const pThis, Point_t *opEye)
{
    Point_copy(opEye, &(pThis->axes.origin));
    return opEye;
}

Vect_t * Camera_getPov(const Camera_t *const pThis, Vect_t *opPov)
{
    Vect_scale(opPov, &(pThis->axes.z), pThis->frame_dist);
    return opPov;
}

Vect_t * Camera_getUp(const Camera_t *const pThis, Vect_t *opUp)
{
    Vect_copy(opUp, &(pThis->axes.y));
    return opUp;
}

Camera_t * Camera_scale(Camera_t *const pThis, const double scale)
{
    Axes_scale(&(pThis->axes), scale);
    return pThis;
}

Camera_t * Camera_yaw(Camera_t *const pThis, const double rads)
{
    Axes_yaw(&(pThis->axes), rads);
    return pThis;
}

Camera_t * Camera_pitch(Camera_t *const pThis, const double rads)
{
    Axes_pitch(&(pThis->axes), rads);
    return pThis;
}

Camera_t * Camera_roll(Camera_t *const pThis, const double rads)
{
    Axes_roll(&(pThis->axes), rads);
    return pThis;
}

Camera_t * Camera_march(Camera_t *const pThis, const double dist)
{
    Axes_march(&(pThis->axes), dist);
    return pThis;
}

Camera_t * Camera_strafe(Camera_t *const pThis, const double dist)
{
    Axes_strafe(&(pThis->axes), dist);
    return pThis;
}

Camera_t * Camera_climb(Camera_t *const pThis, const double dist)
{
    Axes_climb(&(pThis->axes), dist);
    return pThis;
}


