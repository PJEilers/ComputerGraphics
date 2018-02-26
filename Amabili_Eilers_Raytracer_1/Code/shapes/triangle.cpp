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
    ****************************************************/

    
    // Edge vectors
    
    Vector dd = ray.D;
    dd = dd * dd.length();

    Vector e1 = (v2 - v1);
    Vector e2 = (v3 - v1);

    Vector pvec = ray.D.cross(e2);
    double det = e1.dot(pvec);
  

    // To check relative position of the triangle with the ray
    if (fabs(det) < eps) return Hit::NO_HIT();
    

    double invDet = 1.0 / det;
    
    Vector tvec = ray.O - v1;
    double u = tvec.dot(pvec)*invDet;
    
    if (u < 0 || u > 1) return Hit::NO_HIT();
    
    Vector qvec = tvec.cross(e1);
    double v = ray.D.dot(qvec) * invDet;
    
    if (v < 0 || u + v > 1) return Hit::NO_HIT(); 

    double t =  e2.dot(qvec) * invDet;
    
    
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
