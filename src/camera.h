/**
 * File: camera.h
 *
 */
#ifndef CAMERA_H
#define CAMERA_H

#include "axes.h"

typedef struct {

    Axes_t axes;
    double frame_dist;

} Camera_t;

Camera_t * Camera_cfg(Camera_t *const pThis, double frame_dist);

Camera_t * Camera_copy(Camera_t *const pThis, const Camera_t *const pRhs);

Point_t * Camera_getEye(const Camera_t *const pThis, Point_t *opEye);

Vect_t * Camera_getPov(const Camera_t *const pThis, Vect_t *opPov);

Vect_t * Camera_getUp(const Camera_t *const pThis, Vect_t *opUp);

Camera_t * Camera_yaw(Camera_t *const pThis, const double rads);

Camera_t * Camera_pitch(Camera_t *const pThis, const double rads);

Camera_t * Camera_roll(Camera_t *const pThis, const double rads);

Camera_t * Camera_march(Camera_t *const pThis, const double dist);

Camera_t * Camera_strafe(Camera_t *const pThis, const double dist);

Camera_t * Camera_climb(Camera_t *const pThis, const double dist);

#endif
//end inclusion filter


