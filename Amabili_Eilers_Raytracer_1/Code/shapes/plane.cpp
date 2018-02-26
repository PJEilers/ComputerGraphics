#include "plane.h"

#include <cmath>
#define eps 1e-9

using namespace std;

Hit Plane::intersect(Ray const &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, normal.
    * Sought: intersects? if true: *t
    * 
    * Using the ray parametric equation: t = (position -ray.O)*normal/ray.D*normal
    * 
    * 
    ****************************************************/
    

    
    double d = normal.dot(ray.D);
    
    if(d < eps) return Hit::NO_HIT(); //Ray is (close to) parallel to the plane
    
    Vector p = position - ray.O;
    double t = p.dot(normal) / d;
    
    if(t < 0) return Hit::NO_HIT(); //Ray is behind plane
    
    Vector N = normal;
    N.normalize();
    
    return Hit(t,N);
}

Plane::Plane(const Point &pos, const Point &n)
:
    position(pos),
    normal(n)
{}
