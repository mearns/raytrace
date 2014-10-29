/**
 * File: point.c
 *
 */
#include "point.h"

#include <math.h>
#include <stdlib.h>

#include "types.h"
#include "util.h"
#include "vect.h"

Point_t * Point_cfg(Point_t *const pThis, const double x, const double y, const double z)
{
    pThis->x = x;
    pThis->y = y;
    pThis->z = z;
    return pThis;
}

Point_t * Point_copy(Point_t * pThis,  const Point_t *pRhs)
{
    return Point_cfg(pThis, pRhs->x, pRhs->y, pRhs->z);
}

Point_t * Point(const double x, const double y, const double z)
{
    Point_t *const pThis = Util_allocOrDie(sizeof(Point_t), "Allocating a Point_t object.");
    if(pThis != NULL) {
        Point_cfg(pThis, x, y, z);
    }
    return pThis;
}

Point_t * Point_zero(void)
{
    return Point(0,0,0);
}

Point_t * Point_clone(const Point_t *const pRhs)
{
    return Util_cloneOrDie(pRhs, sizeof(Point_t), "Cloning a Point_t object.");
}

Point_t * Point_translate(Point_t *opPoint, const Point_t *pPt, const Vect_t *pTrans)
{
    return Point_cfg(opPoint, pPt->x+pTrans->x, pPt->y+pTrans->y, pPt->z+pTrans->z);
}

Point_t * Point_translateBack(Point_t *opPoint, const Point_t *pPt, const Point_t *pTrans)
{
    return Point_cfg(opPoint, pPt->x-pTrans->x, pPt->y-pTrans->y, pPt->z-pTrans->z);
}

Vect_t * Point_displacement(Vect_t *const opDisp, const Point_t *const pA, const Point_t *const pB)
{
    return Point_cfg(opDisp, pB->x - pA->x, pB->y - pA->y, pB->z - pA->z);
}

double Point_distance(const Point_t *const pThis, const Point_t *const pOther)
{
    double dx, dy, dz;
    const Point_t this = *pThis;
    const Point_t other = *pOther;

    dx = this.x - other.x;
    dy = this.y - other.y;
    dz = this.z - other.z;

    return sqrt(dx*dx + dy*dy + dz*dz);
}


Vect_t * Point_position(Vect_t *opVect, const Point_t *pPt)
{
    return Vect_cfg(opVect, pPt->x, pPt->y, pPt->z);
}

