/**
 * File: triangle.c
 *
 * Module for defining 3-D models.
 */
#include "triangle.h"

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

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

static Color_t * Triangle_getBaryColor(const Triangle_t *const pThis, Color_t *const opColor, const Point_t *const pBary)
{
    const double r = (pBary->x*(pThis->vert[0]->color->r)) + (pBary->y*(pThis->vert[1]->color->r)) + (pBary->z*(pThis->vert[2]->color->r));
    const double g = (pBary->x*(pThis->vert[0]->color->g)) + (pBary->y*(pThis->vert[1]->color->g)) + (pBary->z*(pThis->vert[2]->color->g));
    const double b = (pBary->x*(pThis->vert[0]->color->b)) + (pBary->y*(pThis->vert[1]->color->b)) + (pBary->z*(pThis->vert[2]->color->b));

    return Color_cfg(opColor, r, g, b);
}

double Triangle_intersect(const Triangle_t *pThis, Color_t *opColor, const Point_t *const pt, const Point_t *const vect)
{
    //A point on the plane of the triangle.
    const Point_t *const pop = pThis->vert[0]->loc;

    const double denom = Point_dotProduct(vect, &(pThis->normal));
    if (denom == 0) {
        //Line and plane are parallel, no intersection (or infinite intersection).
        return INFINITY;
    }

    Point_t disp;
    Point_sub(&disp, pop, pt);

    const double numer = Point_dotProduct(&disp, &(pThis->normal));
    const double dist = numer / denom;

    //Intersection if "behind" the starting point of the ray, so there is no intersection.
    if(dist < 0) {
        return INFINITY;
    }

    //Pointer from pt to the intersection.
    Point_t pointer;
    Point_scale(&pointer, vect, dist);

    //Point of intersection.
    Point_t intersection;
    Point_add(&intersection, pt, &pointer);

    //Get the barycentric position of the intersection.
    Point_t bary;
    Triangle_barycentricPosition(pThis, &bary, &intersection);

    //If any of it's components are negative, it is outside of the triangle, so no intersection.
    if(bary.x < 0 || bary.y < 0 || bary.z < 0) {
        //XXX: FIXME: This isn't right. See not at isInside.
        //To get barcentric coord, triangles for calculating areas should always start with P,
        // then go to the other two vertices of the triangle always in a consistent order (the
        // same one used to calculate the total area of the triangle).
        // Then are should be properly signed, and this should work.
        // And I don't remember why I needed to know, but to know if parallel vectors are in
        // the same direction or opposite, take their dot product, which is |U||V|cos(theta),
        // so if they are in the same direction, this will be positive (theta will be 0 or
        // near zero), and if in opposite, it will be negative (theta will be close to 180).
        return INFINITY;
    }

    //Get the color of the intersection point.
    Triangle_getBaryColor(pThis, opColor, &bary);

    return dist;
}

static double Triangle_signedArea(const Point_t *const pNorm, const Point_t *const pA, const Point_t *const pB, const Point_t *const pC)
{
    Point_t ab, ac, xp;

    //Get vectors.
    Point_displacement(&ab, pA, pB);
    Point_displacement(&ac, pA, pC);

    Point_crossProduct(&xp, &ab, &ac);

    const double xpl = Point_dotProduct(&xp, pNorm);

    return 0.5 * xpl;
}

bool Triangle_isInside(const Triangle_t *const pThis, const Point_t *const pPt)
{
    //FIXME: This isn't right, because signedArea isn't actually signed (FIXTHAT, too).
    // We can test if a triangle is lef thanded or right handed by taking the cross product of
    // the two leg vectors extending from a common vertex. The vector points in one direction for
    // right handed, and the other direction for left handed. So we can see if it points in the same (general)
    // direction as the triangle itself, or nearly the opposite.
    Point_t bary;
    Triangle_barycentricPosition(pThis, &bary, pPt);
    if(pPt->x < 0 || pPt->y < 0 || pPt->z < 0) {
        return false;
    }
    return true;
}

Point_t * Triangle_barycentricPosition(const Triangle_t *const pThis, Point_t *const opBarry, const Point_t *const pPoint)
{
    const Point_t *const a = pThis->vert[0]->loc;
    const Point_t *const b = pThis->vert[1]->loc;
    const Point_t *const c = pThis->vert[2]->loc;

    const double pbc = Triangle_signedArea(&(pThis->normal), pPoint, b, c);
    const double pca = Triangle_signedArea(&(pThis->normal), a, pPoint, c);
    const double pab = Triangle_signedArea(&(pThis->normal), a, b, pPoint);

    return Point_cfg(opBarry, pbc / pThis->area, pca / pThis->area, pab / pThis->area);
}

Triangle_t* Triangle_cfg(Triangle_t *const pThis, Vertex_t *const pVertex1, Vertex_t *const pVertex2, Vertex_t *const pVertex3)
{
    pThis->vert[0] = pVertex1;
    pThis->vert[1] = pVertex2;
    pThis->vert[2] = pVertex3;

    Point_t u, v;
    Point_displacement(&u, pVertex1->loc, pVertex2->loc);
    Point_displacement(&v, pVertex1->loc, pVertex3->loc);
    Point_crossProduct(&(pThis->normal), &u, &v);
    Point_normalize(&(pThis->normal), &(pThis->normal));

    pThis->area = Triangle_signedArea(&(pThis->normal), pVertex1->loc, pVertex2->loc, pVertex3->loc);

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
    return Triangle_getBaryColor(pThis, opColor, &bary);
};



