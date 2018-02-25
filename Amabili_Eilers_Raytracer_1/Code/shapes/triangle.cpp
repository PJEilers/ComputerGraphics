#include "triangle.h"

#include <cmath>
#define eps 1e-9

using namespace std;

Hit Triangle::intersect(Ray const &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray.0, ray.D, v1, v2, v3
    * Sought: intersects? if true: *t
    *
    * The ray parametric equation is:
    *       ray = p + t * d
    *
    * where d = distance between ray and position
    *       p = point in the line
    *       t = vector that provides the line's direction 
    *
    * Considering that 
    *       point(u,v) = (1-u-v)*p0 + u*p1 + v*p2
    *
	*       where p0,p1,p2 are the vertices of the triangle 
    *
	*       u >= 0
	*       v >= 0
	*       u + v <= 1
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    // place holder for actual intersection calculation
    
    // Edge vectors

    
    Vector e1 = (v2 - v1);
    Vector e2 = (v3 - v1);

    // Face normal - Normal calculation
    Vector N = e1.cross(e2);
  
    double q = N.dot(ray.D);

    // To check relative position of the triangle with the ray
    if (fabs(q) < eps) return Hit::NO_HIT();

    double d = N.dot(v1);

    double t =- ((N.dot(ray.O) + d) / q); 


    // To check if the triangle is in behind the ray
    if (t < 0) return Hit::NO_HIT();

   

    Vector P = ray.O + (t * ray.D);

    // To test whether the intersection is outside the triangle
    // Checking edge by edge
    Vector edge1 = (v2 - v1);
    Vector edge2 = (v3 - v2);
    Vector edge3 = (v1 - v3);
    Vector C1 = P - v1;
    Vector C2 = P - v2;
    Vector C3 = P - v3;

    if (N.dot(edge1.cross(C1)) < 0 ||
        N.dot(edge2.cross(C2)) < 0 ||
        N.dot(edge3.cross(C3)) < 0) return Hit::NO_HIT();
       

                 

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    *
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/

    N.normalize();

    return Hit(t, N);
}

Triangle::Triangle(Point const &v1, Point const &v2, Point const &v3)
:
    v1(v1),
    v2(v2),
    v3(v3)
{}
