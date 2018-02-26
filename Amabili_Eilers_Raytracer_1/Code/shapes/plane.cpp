#include "plane.h"

#include <cmath>

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
    
    if(d < 0.000001) return Hit::NO_HIT(); //Ray is (close to) parallel to the plane
    
    Vector p = position - ray.O;
    double t = p.dot(normal) / d;
    
    if(t < 0) return Hit::NO_HIT(); //Ray is behind plane
    
    normal.normalize();
    
    return Hit(t,normal);
}

Plane::Plane(Point &pos, Point &n)
:
    position(pos),
    normal(n)
{}
