/**
 * File: quats.c
 *
 */
#include "quats.h"

#include <math.h>

#include "util.h"
#include "point.h"
#include "vect.h"

Quat_t * Quat_cfg(Quat_t *pThis, double w, double x, double y, double z)
{
    pThis->w = w;
    pThis->x = x;
    pThis->y = y;
    pThis->z = z;
    return pThis;
}

Quat_t * Quat_copy(Quat_t *pThis, const Quat_t *pRhs)
{
    return Quat_cfg(pThis, pRhs->w, pRhs->x, pRhs->y, pRhs->z);
}

Quat_t * Quat(double w, double x, double y, double z)
{
    Quat_t *const pThis = Util_allocOrDie(sizeof(Quat_t), "Allocating a Quat_t object.");
    if(pThis != NULL) {
        Quat_cfg(pThis, w, x, y, z);
    }
    return pThis;
}

Quat_t * Quat_clone(const Quat_t *pRhs)
{
    return Util_cloneOrDie(pRhs, sizeof(Quat_t), "Cloning a Quat_t object.");
}

Quat_t * Quat_zero(void)
{
    return Quat(0, 0, 0, 0);
}


Quat_t * Quat_rotation(Quat_t *pThis, const Vect_t *pAxis, double rads)
{
    //For rotation by A rads around unit-vector axis (x,y,z), we use the quat
    // (cos(A/2), x*sin(A/2), y*sin(A/2), z*sin(A/2)).

    double lat;
    double length;
    Vect_t imag;
    
    //Get the "latitude"-ish, just half the angle.
    lat = rads * 0.5;

    //Get the length of the axis vector
    length = Vect_magnitude(pAxis);

    //And scale the vector appropriately.
    Vect_scale(&imag, pAxis, sin(lat)/length);

    return Quat_cfg(pThis, cos(lat), imag.x, imag.y, imag.z);
}

