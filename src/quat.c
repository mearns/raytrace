/**
 * File: quats.c
 *
 */
#include "quat.h"

#include <math.h>

#include "util.h"
#include "point.h"
#include "vect.h"
#include "vertex.h"
#include "triangle.h"

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

Quat_t * Quat_product(Quat_t *opThis, const Quat_t *pA, const Quat_t *pB)
{
    const double a1 = pA->w;
    const double b1 = pA->x;
    const double c1 = pA->y;
    const double d1 = pA->z;

    const double a2 = pB->w;
    const double b2 = pB->x;
    const double c2 = pB->y;
    const double d2 = pB->z;

    return Quat_cfg(
        opThis,
        a1*a2 - b1*b2 - c1*c2 - d1*d2,
        a1*b2 + b1*a2 + c1*d2 - d1*c2,
        a1*c2 - b1*d2 + c1*a2 + d1*b2,
        a1*d2 + b1*c2 - c1*b2 + d1*a2
    );
}

Quat_t * Quat_conjugate(Quat_t *opThis, const Quat_t *pRhs)
{
    return Quat_cfg(opThis, pRhs->w, -(pRhs->x), -(pRhs->y), -(pRhs->z));
}

static void Quat_rotateTuple(const Quat_t *pThis, double coords[3])
{
    Quat_t pt, conj, prod, result;

    //Turn the point into a quat.
    Quat_cfg(&pt, 0, coords[0], coords[1], coords[2]);

    //Conjugate the point-quat with the rotation quat.
    Quat_conjugate(&conj, pThis);
    Quat_product(&prod, pThis, &pt);
    Quat_product(&result, &prod, &conj);

    //Extract the point from the product.
    coords[0] = result.x;
    coords[1] = result.y;
    coords[2] = result.z;
}

Point_t * Quat_rotatePoint(const Quat_t *pThis, Point_t *opRotated, const Point_t *pPoint)
{
    double coords[3] = {pPoint->x, pPoint->y, pPoint->z};
    Quat_rotateTuple(pThis, coords);
    return Point_cfg(opRotated, coords[0], coords[1], coords[2]);
}

Vect_t * Quat_rotateVect(const Quat_t *pThis, Vect_t *opRotated, const Vect_t *pVect)
{
    double coords[3] = {pVect->x, pVect->y, pVect->z};
    Quat_rotateTuple(pThis, coords);
    return Vect_cfg(opRotated, coords[0], coords[1], coords[2]);
}

Vertex_t * Quat_rotateVertexInPlace(const Quat_t *pThis, Vertex_t *pVertex)
{
    double coords[3] = {pVertex->loc.x, pVertex->loc.y, pVertex->loc.z};
    Quat_rotateTuple(pThis, coords);
    Point_cfg(&(pVertex->loc), coords[0], coords[1], coords[2]);
    return pVertex;
}

Triangle_t * Quat_rotateTriangleInPlace(const Quat_t *pThis, Triangle_t *pTriangle)
{
    Quat_rotateVertexInPlace(pThis, pTriangle->vert[0]);
    Quat_rotateVertexInPlace(pThis, pTriangle->vert[1]);
    Quat_rotateVertexInPlace(pThis, pTriangle->vert[2]);
    return pTriangle;
}


