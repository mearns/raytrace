/**
 * File: model.h
 *
 */
#ifndef MODEL_H
#define MODEL_H

#include "axes.h"
#include "triangle.h"

typedef struct Model Model_t;

struct Model {
    Axes_t axes;
    Model_t *children;
    Triangle_t *triangles;
};

Model_t * Model_cfg(Model_t *pThis);

#endif
//end inclusion filter

