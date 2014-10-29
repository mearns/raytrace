/**
 * File: axes.h
 *
 */
#ifndef AXES_H
#define AXES_H

#include "vect.h"
#include "point.h"
#include "quat.h"

typedef struct {
    Vect_t x;
    Vect_t y;
    Vect_t z;
    Point_t origin;
} Axes_t;

Axes_t * Axes_cfg(Axes_t *pThis);

Axes_t * Axes(void);

Axes_t * Axes_copy(Axes_t *pThis, const Axes_t *const pRhs);

Axes_t * Axes_clone(const Axes_t *const pRhs);

Axes_t * Axes_yaw(Axes_t *const pThis, const double rads);

Axes_t * Axes_pitch(Axes_t *const pThis, const double rads);

Axes_t * Axes_roll(Axes_t *const pThis, const double rads);

Axes_t * Axes_march(Axes_t *const pThis, const double dist);

Axes_t * Axes_strafe(Axes_t *const pThis, const double dist);

Axes_t * Axes_climb(Axes_t *const pThis, const double dist);

Point_t * Axes_point(const Axes_t *const pThis, Point_t *const opGlobal, const Point_t *const pLocal);

Vect_t * Axes_vect(const Axes_t *const pThis, Vect_t *const opGlobal, const Vect_t *const pLocal);

#endif
//end inclusion filter


