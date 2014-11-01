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

/**
 * Function: Triangle_getBaryColor
 * Gets the color for a point on the triangle based on it's barycentric coordinates.
 *
 * Arguments:
 *  pThis   -   const <Triangle_t>* : Pointer to the triangle.
 *  opColor -   <Color_t>* : Pointer to the object that will hold the output color.
 *  pBary   -   const <Point_t>* : A point containing the barycentric coordinates of the point.
 */
static Color_t * Triangle_getBaryColor(const Triangle_t *const pThis, Color_t *const opColor, const Point_t *const pBary)
{
    const double r = (pBary->x*(pThis->vert[0]->color.r)) + (pBary->y*(pThis->vert[1]->color.r)) + (pBary->z*(pThis->vert[2]->color.r));
    const double g = (pBary->x*(pThis->vert[0]->color.g)) + (pBary->y*(pThis->vert[1]->color.g)) + (pBary->z*(pThis->vert[2]->color.g));
    const double b = (pBary->x*(pThis->vert[0]->color.b)) + (pBary->y*(pThis->vert[1]->color.b)) + (pBary->z*(pThis->vert[2]->color.b));

    return Color_cfg(opColor, r, g, b);
}

double Triangle_rayCast(const Triangle_t *pThis, Color_t *opColor, const double closest_dist, const Point_t *const pt, const Vect_t *const vect)
{
    Vect_t disp;
    Vect_t pointer;
    Point_t intersection;
    Point_t bary;

    //A point on the plane of the triangle.
    const Point_t *const pop = &(pThis->vert[0]->loc);

    //Convenient handle to the plane-normal.
    const Vect_t *const pNorm = &(pThis->normal);

    // What we're doing here is intersecting a ray with a plane. 
    // The vector is a parametric equation like: pt + t*vect, so what we're going
    // to find is the value of t for which the ray and the plane coincide.
    // This is done as t = (disp . norm) / (vect . norm), where disp is a vector
    // from the start of the ray to any point on the plane.
    
    //Get the denominator for the distance calc.
    const double denom = Vect_dot(vect, pNorm);
    if (denom == 0) {
        //Line and plane are parallel, no intersection (or infinite intersection).
        return closest_dist;
    }

    // Finish the distance calc.
    Point_displacement(&disp, pt, pop);
    const double numer = Vect_dot(&disp, pNorm);
    const double dist = numer / denom;

    //If t is negative, then the intersection if "behind" the starting point of the ray, so there is no intersection.
    // Or, if the intersection is further away than a point we've already hit with the ray, then we don't care about this
    // one, we use the other one.
    if(dist < 0 || dist >= closest_dist) {
        return closest_dist;
    }

    //Pointer from pt to the intersection.
    Vect_scale(&pointer, vect, dist);

    //Point of intersection.
    Point_translate(&intersection, pt, &pointer);

    //Get the barycentric position of the intersection.
    Triangle_barycentricPosition(pThis, &bary, &intersection);

    //If any of it's components are negative, it is outside of the triangle, so no intersection.
    if(bary.x < 0 || bary.y < 0 || bary.z < 0) {
        return closest_dist;
    }

    //Get the color of the intersection point.
    Triangle_getBaryColor(pThis, opColor, &bary);

    //This is now the closest distance.
    return dist;
}

/**
 * Function: Triangle_signedArea
 * Gets the signed area of a triangle defined by three points, relative to an orientation
 * defining normal vector.
 *
 * The triangle is defined by the three point vetices, A, B, and C. The given normal vector
 * should be (approximately) normal to the triangle and indicates what orientation is considered
 * to have positive area versus negative area. When looking along the direction of the vector,
 * if the vertices A, B, and C are arranged clockwise (in that order) then the are is positive,
 * otherwise it is negative. I'm pretty sure I got that right, otherwise it's the reverse.
 */
static double Triangle_signedArea(const Vect_t *const pNorm, const Point_t *const pA, const Point_t *const pB, const Point_t *const pC)
{
    Vect_t ab, ac, xp;

    //Get vectors.
    Point_displacement(&ab, pA, pB);
    Point_displacement(&ac, pA, pC);

    Vect_cross(&xp, &ab, &ac);

    const double xpl = Vect_dot(&xp, pNorm);

    return 0.5 * xpl;
}

bool Triangle_isInside(const Triangle_t *const pThis, const Point_t *const pPt)
{
    Point_t bary;
    Triangle_barycentricPosition(pThis, &bary, pPt);
    if(pPt->x < 0 || pPt->y < 0 || pPt->z < 0) {
        return false;
    }
    return true;
}

Point_t * Triangle_barycentricPosition(const Triangle_t *const pThis, Point_t *const opBarry, const Point_t *const pPoint)
{
    const Point_t *const a = &(pThis->vert[0]->loc);
    const Point_t *const b = &(pThis->vert[1]->loc);
    const Point_t *const c = &(pThis->vert[2]->loc);

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

    Vect_t u, v;
    Point_displacement(&u, &(pVertex1->loc), &(pVertex2->loc));
    Point_displacement(&v, &(pVertex1->loc), &(pVertex3->loc));
    Vect_cross(&(pThis->normal), &u, &v);
    Vect_normalize(&(pThis->normal), &(pThis->normal));

    pThis->area = Triangle_signedArea(&(pThis->normal), &(pVertex1->loc), &(pVertex2->loc), &(pVertex3->loc));

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



