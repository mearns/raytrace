/**
 * File: triangle.c
 *
 * Module for defining 3-D models.
 */
#include "triangle.h"

#include <stdint.h>
#include <math.h>

#include "point.h"
#include "util.h"

static const TriangleVertIndex_t Triangle_OtherVerts[3][2] = {
    {VTX2, VTX3},
    {VTX1, VTX3},
    {VTX1, VTX2}
};

static const TriangleVertIndex_t Triangle_Sides[3][2] = {
    //Order matters, must match Triangle_OtherVerts.
    {VTX1, VTX3},   //(1->2), (1->3)
    {VTX1, VTX2},   //(2->1), (2->3)
    {VTX3, VTX2}    //(3->1), (3->2)
};

static TriangleVertIndex_t vertIndex(int vidx)
{
    if(vidx < 0) {
        return VTX1;
    }
    if(vidx > 2) {
        return VTX3;
    }
    return (TriangleVertIndex_t)(vidx);
}

static double Triangle_signedArea(const Point_t *const pA, const Point_t *const pB, const Point_t *const pC)
{
    Point_t ab, ac;

    //Get vectors.
    Point_displacement(&ab, pA, pB);
    Point_displacement(&ac, pA, pC);

    return 0.5 * Point_length(&ab) * Point_length(&ac) * sin(Point_vectorAngle(&ab, &ac));
}

Point_t * Triangle_barycentricPosition(const Triangle_t *const pThis, Point_t *const opBarry, const Point_t *const pPoint)
{
    const Point_t *const a = pThis->vert[0]->loc;
    const Point_t *const b = pThis->vert[1]->loc;
    const Point_t *const c = pThis->vert[2]->loc;

    const double pbc = Triangle_signedArea(pPoint, b, c);
    const double pca = Triangle_signedArea(pPoint, c, a);
    const double pab = Triangle_signedArea(pPoint, a, b);

    return Point_cfg(opBarry, pbc / pThis->area, pca / pThis->area, pab / pThis->area);
}

Triangle_t* Triangle_cfg(Triangle_t *const pThis, Vertex_t *const pVertex1, Vertex_t *const pVertex2, Vertex_t *const pVertex3)
{
    pThis->vert[0] = pVertex1;
    pThis->vert[1] = pVertex2;
    pThis->vert[2] = pVertex3;

    pThis->side[0] = Point_distance(pVertex1->loc, pVertex2->loc);
    pThis->side[1] = Point_distance(pVertex2->loc, pVertex3->loc);
    pThis->side[2] = Point_distance(pVertex3->loc, pVertex1->loc);

    const double a = pVertex1->loc->x - pVertex3->loc->x;
    const double b = pVertex2->loc->x - pVertex3->loc->x;
    const double c = pVertex1->loc->y - pVertex3->loc->y;
    const double d = pVertex2->loc->y - pVertex3->loc->y;
    const double inv_det = 1.0 / ((a*d) - (b*c));

    pThis->inv_barryform[0][0] = d*inv_det;
    pThis->inv_barryform[1][0] = (-b)*inv_det;
    pThis->inv_barryform[0][1] = (-c)*inv_det;
    pThis->inv_barryform[1][1] = a*inv_det;

    pThis->area = Triangle_signedArea(pVertex1->loc, pVertex2->loc, pVertex3->loc);

    return pThis;
}

Triangle_t* Triangle(Vertex_t *const pVertex1, Vertex_t *const pVertex2, Vertex_t *const pVertex3)
{
    Triangle_t *pThis = Util_allocOrDie(sizeof(Triangle_t), "Allocating new Triangle_t object.");
    if(pThis != NULL) {
        Triangle_cfg(pThis, pVertex1, pVertex2, pVertex3);
    }
    return pThis;
}

Triangle_t* Triangle_clone(const Triangle_t *const pRhs)
{
    return Util_cloneOrDie(pRhs, sizeof(Triangle_t), "Cloning Triangle_t object.");
}

Color_t * Triangle_getColor(const Triangle_t *const pThis, Color_t *const opColor, const Point_t *const pPt)
{
    Point_t bary;
    Triangle_barycentricPosition(pThis, &bary, pPt);

    const double r = (bary.x*(pThis->vert[0]->color->r)) + (bary.y*(pThis->vert[1]->color->r)) + (bary.z*(pThis->vert[2]->color->r));
    const double g = (bary.x*(pThis->vert[0]->color->g)) + (bary.y*(pThis->vert[1]->color->g)) + (bary.z*(pThis->vert[2]->color->g));
    const double b = (bary.x*(pThis->vert[0]->color->b)) + (bary.y*(pThis->vert[1]->color->b)) + (bary.z*(pThis->vert[2]->color->b));

    return Color_cfg(opColor, r, g, b);
};



