#include "triangle.h"

#include <cmath>
#define eps 1e-9

using namespace std;

Hit Triangle::intersect(Ray const &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION AND NORMAL CALCULATION
    *
    * Given: ray.0, ray.D, v1, v2, v3
    * Sought: intersects? if true: *t
    *
    * Using the Möller-Trumbore algorithm
    * 
    * https://en.wikipedia.org/wiki/MollerTrumbore_intersection_algorithm
    *
    ****************************************************/

    
    // Edge vectors

    Vector e1 = (v2 - v1);
    Vector e2 = (v3 - v1);
    
    //Calculating determinant

    Vector pv = ray.D.cross(e2);
    double det = e1.dot(pv);
  
    // Checking if ray is (almost) parallel to triangle
    
    if (fabs(det) < eps) return Hit::NO_HIT();
    
    
    //Inverse of determinant
    
    double invDet = 1.0 / det;
    
    Vector tv = ray.O - v1;
    double u = tv.dot(pv)*invDet;
    
    if (u < 0 || u > 1) return Hit::NO_HIT();
    
    Vector qv = tv.cross(e1);
    double v = ray.D.dot(qv) * invDet;
    
    if (v < 0 || u + v > 1) return Hit::NO_HIT(); 

    double t =  e2.dot(qv) * invDet;
    
    // Normal calculation
    
    Vector N = e1.cross(e2);
    N.normalize();

    return Hit(t, N);
}

Triangle::Triangle(Point const &v1, Point const &v2, Point const &v3)
:
    v1(v1),
    v2(v2),
    v3(v3)
{}
