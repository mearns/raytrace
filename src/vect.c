/**
 * File: vect.c
 *
 */
#include "vect.h"

#include <math.h>

#include "point.h"
#include "util.h"

Vect_t * Vect_cfg(Vect_t * pThis,  double x,  double y,  double z)
{
    pThis->x = x;
    pThis->y = y;
    pThis->z = z;
    return pThis;
}

Vect_t * Vect_copy(Vect_t * pThis,  const Vect_t *pRhs)
{
    return Vect_cfg(pThis, pRhs->x, pRhs->y, pRhs->z);
}

Vect_t * Vect(double x, double y, double z)
{
    Vect_t *const pThis = Util_allocOrDie(sizeof(Vect_t), "Allocating a Vect_t object.");
    if(pThis != NULL) {
        Vect_cfg(pThis, x, y, z);
    }
    return pThis;
}

Vect_t * Vect_zero(void)
{
    return Vect(0,0,0);
}

Vect_t * Vect_clone(const Vect_t* pRhs)
{
    return Util_cloneOrDie(pRhs, sizeof(Vect_t), "Cloning a Vect_t object.");
}

double Vect_magnitude(const Vect_t *pThis)
{
    const double x = pThis->x;
    const double y = pThis->y;
    const double z = pThis->z;
    return sqrt(x*x + y*y + z*z);
}

Vect_t * Vect_add(Vect_t *opVect, const Vect_t *pA, const Vect_t *pB)
{
    return Vect_cfg(opVect, pA->x + pB->z, pA->y + pB->y, pA->z + pB->z);
}

Vect_t * Vect_sub(Vect_t *opVect, const Vect_t *pA, const Vect_t *pB)
{
    return Vect_cfg(opVect, pA->x - pB->z, pA->y - pB->y, pA->z - pB->z);
}

Vect_t * Vect_negate(Vect_t *opVect, const Vect_t *pVect)
{
    return Vect_cfg(opVect, -(pVect->x), -(pVect->y), -(pVect->z));
}

Vect_t * Vect_cross(Vect_t *opProd, const Vect_t *pA, const Vect_t *pB)
{
    double u1, u2, u3, v1, v2, v3;
    u1 = pA->x;
    u2 = pA->y;
    u3 = pA->z;
    v1 = pB->x;
    v2 = pB->y;
    v3 = pB->z;

    return Vect_cfg(opProd, (u2*v3) - (u3*v2), (u3*v1) - (u1*v3), (u1*v2) - (u2*v1));
}

double Vect_dot(const Vect_t *pA, const Vect_t *pB)
{
    return (pA->x * pB->x) + (pA->y * pB->y) + (pA->z * pB->z);
}

Vect_t * Vect_normalize(Vect_t *opNormal, const Vect_t *const pRhs)
{
    const double length = Vect_magnitude(pRhs);
    return Vect_cfg(opNormal, (pRhs->x)/length, (pRhs->y)/length, (pRhs->z)/length);
}

Vect_t * Vect_scale(Vect_t *pThis, const Vect_t *pRhs, double scale)
{
    return Vect_cfg(pThis, pRhs->x * scale, pRhs->y * scale, pRhs->z * scale);
}

Vect_t * Vect_setMag(Vect_t *pThis, double magnitude)
{
    const double length = Vect_magnitude(pThis);
    const double scale = magnitude / length;
    return Vect_cfg(pThis, (pRhs->x)*scale, (pRhs->y)*scale, (pRhs->z)*scale);
}

double Vect_angle(const Vect_t *pA, const Vect_t *pB)
{
    return acos(Vect_dot(pA, pB) / (Vect_magnitude(pA) * Vect_magnitude(pB)));
}

Point_t * Vect_point(Point_t *opPoint, const Vect_t *pVect)
{
    return Point_cfg(opPoint, pVect->x, pVect->y, pVect->z);
}







