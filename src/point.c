/**
 * File: point.c
 *
 */
#include "point.h"

#include <math.h>
#include <stdlib.h>

#include "util.h"

Point_t * Point(const double x, const double y, const double z)
{
    Point_t *const pThis = Util_allocOrDie(sizeof(Point_t), "Allocating a Point_t object.");
    if(pThis != NULL) {
        Point_cfg(pThis, x, y, z);
    }
    return pThis;
}

Point_t * Point_clone(const Point_t *const pRhs)
{
    return Util_cloneOrDie(pRhs, sizeof(Point_t), "Cloning a Point_t object.");
}

Point_t * Point_cfg(Point_t *const pThis, const double x, const double y, const double z)
{
    pThis->x = x;
    pThis->y = y;
    pThis->z = z;
    return pThis;
}

Point_t * Point_displacement(Point_t *const opDisp, const Point_t *const pA, const Point_t *const pB)
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

double Point_length(const Point_t *const pThis)
{
    const double x = pThis->x;
    const double y = pThis->y;
    const double z = pThis->z;
    return sqrt(x*x + y*y + z*z);
}

double Point_dotProduct(const Point_t *const pA, const Point_t *const pB)
{
    return (pA->x * pB->x) + (pA->y * pB->y) + (pA->z * pB->z);
}

Point_t * Point_crossProduct(Point_t *opProd, const Point_t *pA, const Point_t *pB)
{
    double u1, u2, u3, v1, v2, v3;
    u1 = pA->x;
    u2 = pA->y;
    u3 = pA->z;
    v1 = pB->x;
    v2 = pB->y;
    v3 = pB->z;

    return Point_cfg(opProd, (u2*v3) - (u3*v2), (u3*v1) - (u1*v3), (u1*v2) - (u2*v1));
}

Point_t * Point_normalize(Point_t *opNormal, const Point_t *const pRhs)
{
    const double length = Point_length(pRhs);
    return Point_cfg(opNormal, (pRhs->x)/length, (pRhs->y)/length, (pRhs->z)/length);
}

double Point_vectorAngle(const Point_t *pA, const Point_t *pB)
{
    return acos((Point_dotProduct(pA, pB) / Point_length(pA)) / Point_length(pB));
}

Point_t * Point_scale(Point_t *pThis, const Point_t *pRhs, double scale)
{
    return Point_cfg(pThis, pRhs->x * scale, pRhs->y * scale, pRhs->z * scale);
}

