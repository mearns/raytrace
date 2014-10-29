/**
 * File: axes.c
 *
 */
#include "axes.h"

#include "vect.h"
#include "point.h"
#include "util.h"

Axes_t * Axes_cfg(Axes_t *const pThis)
{
    Vect_cfg(&(pThis->x), 1, 0, 0);
    Vect_cfg(&(pThis->y), 0, 1, 0);
    Vect_cfg(&(pThis->z), 0, 0, 1);
    Point_cfg(&(pThis->origin), 0, 0, 0);

    return pThis;
}

Axes_t * Axes(void)
{
    Axes_t *const pThis = Util_allocOrDie(sizeof(Axes_t), "Allocating a Axes_t object.");
    if(pThis != NULL) {
        Axes_cfg(pThis);
    }
    return pThis;
}

Axes_t * Axes_copy(Axes_t *const pThis, const Axes_t *const pRhs)
{
    Vect_copy(&(pThis->x), &(pRhs->x));
    Vect_copy(&(pThis->y), &(pRhs->y));
    Vect_copy(&(pThis->z), &(pRhs->z));
    Point_copy(&(pThis->origin), &(pRhs->origin));
    return pThis;
}

Axes_t * Axes_clone(const Axes_t *const pRhs)
{
    return Util_cloneOrDie(pRhs, sizeof(Axes_t), "Cloning a Axes_t object.");
}

Axes_t * Axes_yaw(Axes_t *const pThis, const double rads)
{
    Quat_t rot;
    Quat_rotation(&rot, &(pThis->y), rads);
    Quat_rotateVect(&rot, &(pThis->x), &(pThis->x));
    Vect_cross(&(pThis->z), &(pThis->x), &(pThis->y));
    return pThis;
}

Axes_t * Axes_pitch(Axes_t *const pThis, const double rads)
{
    Quat_t rot;
    Quat_rotation(&rot, &(pThis->x), rads);
    Quat_rotateVect(&rot, &(pThis->y), &(pThis->y));
    Vect_cross(&(pThis->z), &(pThis->x), &(pThis->y));
    return pThis;
}

Axes_t * Axes_roll(Axes_t *const pThis, const double rads)
{
    Quat_t rot;
    Quat_rotation(&rot, &(pThis->z), rads);
    Quat_rotateVect(&rot, &(pThis->y), &(pThis->y));
    Vect_cross(&(pThis->x), &(pThis->y), &(pThis->z));
    return pThis;
}

Axes_t * Axes_march(Axes_t *const pThis, const double dist)
{
    Vect_t disp;
    Vect_scale(&disp, &(pThis->z), dist);
    Point_translate(&(pThis->origin), &(pThis->origin), &disp);
    return pThis;
}

Axes_t * Axes_strafe(Axes_t *const pThis, const double dist)
{
    Vect_t disp;
    Vect_scale(&disp, &(pThis->x), dist);
    Point_translate(&(pThis->origin), &(pThis->origin), &disp);
    return pThis;
}

Axes_t * Axes_climb(Axes_t *const pThis, const double dist)
{
    Vect_t disp;
    Vect_scale(&disp, &(pThis->y), dist);
    Point_translate(&(pThis->origin), &(pThis->origin), &disp);
    return pThis;
}

Point_t * Axes_point(const Axes_t *const pThis, Point_t *const opGlobal, const Point_t *const pLocal)
{
    Vect_t sum;
    Vect_t component;

    Vect_scale(&sum, &(pThis->x), pLocal->x);
    Vect_add(&sum, &sum, Vect_scale(&component, &(pThis->y), pLocal->y));
    Vect_add(&sum, &sum, Vect_scale(&component, &(pThis->z), pLocal->z));

    return Point_translate(opGlobal, &(pThis->origin), &sum);
}

Vect_t * Axes_vect(const Axes_t *const pThis, Vect_t *const opGlobal, const Vect_t *const pLocal)
{
    Vect_t sum;
    Vect_t component;
    Vect_t ovect;

    Vect_scale(&sum, &(pThis->x), pLocal->x);
    Vect_add(&sum, &sum, Vect_scale(&component, &(pThis->y), pLocal->y));
    Vect_add(&sum, &sum, Vect_scale(&component, &(pThis->z), pLocal->z));

    return Vect_add(opGlobal, Point_position(&ovect, &(pThis->origin)), &sum);

}

