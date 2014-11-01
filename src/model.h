/**
 * File: model.h
 *
 */
#ifndef MODEL_H
#define MODEL_H

#include "axes.h"
#include "triangle.h"

typedef struct Model Model_t;

//TODO: Need to add a method to axes to turn a global point into a local one.
// Then we can take an incoming ray and make it a local ray, and use that to
// intersect all the triangles and children, then make the resulting intersection
// global again.

struct Model {
    Axes_t axes;
    Model_t *children;
    Triangle_t *triangles;
};

Model_t * Model_cfg(Model_t *pThis);

Model_t * Model_yaw(Model_t *const pThis, const double rads);

Model_t * Model_pitch(Model_t *const pThis, const double rads);

Model_t * Model_roll(Model_t *const pThis, const double rads);

Model_t * Model_march(Model_t *const pThis, const double dist);

Model_t * Model_strafe(Model_t *const pThis, const double dist);

Model_t * Model_climb(Model_t *const pThis, const double dist);



#endif
//end inclusion filter

