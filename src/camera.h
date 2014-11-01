/**
 * File: camera.h
 *
 */
#ifndef CAMERA_H
#define CAMERA_H

#include "axes.h"

typedef struct {

    /**
     * Field: axes
     *
     * Defines the local coordinate system and orientation of the camera.
     * So if you scale, translate, or rotate this axes, you're moving the
     * camera. The eye of the camera is at the origin, and the POV looks
     * along the Z axes, with the Y axes defining up.
     */
    Axes_t axes;

    /**
     * Field: frame_dist
     *
     * The distance from the eye to the frame, measured in the local
     * coordinate system.
     */
    double frame_dist;

} Camera_t;

Camera_t * Camera_cfg(Camera_t *const pThis, double frame_dist);

Camera_t * Camera_copy(Camera_t *const pThis, const Camera_t *const pRhs);

Point_t * Camera_getEye(const Camera_t *const pThis, Point_t *opEye);

Vect_t * Camera_getPov(const Camera_t *const pThis, Vect_t *opPov);

Vect_t * Camera_getUp(const Camera_t *const pThis, Vect_t *opUp);

Camera_t * Camera_scale(Camera_t *const pThis, const double scale);

Camera_t * Camera_yaw(Camera_t *const pThis, const double rads);

Camera_t * Camera_pitch(Camera_t *const pThis, const double rads);

Camera_t * Camera_roll(Camera_t *const pThis, const double rads);

Camera_t * Camera_march(Camera_t *const pThis, const double dist);

Camera_t * Camera_strafe(Camera_t *const pThis, const double dist);

Camera_t * Camera_climb(Camera_t *const pThis, const double dist);

#endif
//end inclusion filter


