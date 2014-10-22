/**
 * File: plane.c
 *
 */
#include "plane.h"
#include "point.h"

#include "util.h"

Plane_t * Plane(double a, double b, double c, double d)
{
    Plane_t *pThis = Util_allocOrDie(sizeof(Plane_t), "Allocating a new Plane_t object.");
    if(pThis != NULL) {
        Plane_cfg(pThis, a, b, c, d);
    }
    return pThis;
}

Plane_t * Plane_clone(const Plane_t *const pRhs)
{
    return Util_cloneOrDie(pRhs, sizeof(Plane_t), "Cloning a Plane_t object.");
}

Plane_t * Plane_cfg(Plane_t *const pThis, double a, double b, double c, double d)
{
    pThis->params[0] = a;
    pThis->params[1] = b;
    pThis->params[2] = c;
    pThis->params[3] = d;
    return pThis;
}

Point_t * Plane_projectPoint(const Plane_t *const pThis, Point_t *const opProj, const Point_t *const pPoint)
{
    Point_t pop;
    Point_t norm;

    //Get the normal unit vector.
    Plane_getNormal(pThis, &norm);

    //Get a point on the plane.
    Plane_getPoint(pThis, &pop);

    Point_t disp;
    Point_displacement(&disp, &pop, pPoint);

    const double dot = Point_dotProduct(&disp, &norm);

    Point_t scaled;
    Point_scale(&scaled, &norm, dot);

    return Point_displacement(opProj, &scaled, pPoint);
}

Point_t * Plane_getNormal(const Plane_t *const pThis, Point_t *const opNormal)
{
    return Point_cfg(opNormal, pThis->params[0], pThis->params[1], pThis->params[2]);
}

Point_t * Plane_getPoint(const Plane_t *const pThis, Point_t *opPoint)
{
    return Point_cfg(opPoint, 0, 0, pThis->params[4] / pThis->params[3]);
}

Plane_t * Plane_cfgFromThreePoints(Plane_t *const pThis, const Point_t *pt1, const Point_t *pt2, const Point_t *pt3)
{
    Point_t vec1, vec2, norm;

    //Get vectors from one point to each of the others.
    Point_displacement(&vec1, pt1, pt2);
    Point_displacement(&vec2, pt1, pt3);

    //Cross product to get normal vector.
    Point_crossProduct(&norm, &vec1, &vec2);
    Point_normalize(&norm, &norm);

    const double a = norm.x;
    const double b = norm.y;
    const double c = norm.z;

    //ax + by + cz = -d
    const double d = -((a*pt3->x) + (b*pt3->y) + (c*pt3->z));

    return Plane_cfg(pThis, a, b, c, d);
}





