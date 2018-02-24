#include "sphere.h"

#include <cmath>

using namespace std;

Hit Sphere::intersect(Ray const &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    *
    * Insert calculation of ray/sphere intersection here.
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    // place holder for actual intersection calculation

    Vector OC = (position - ray.O);
    double tca = OC.dot(ray.D);
    if (tca < 0) {
        return Hit::NO_HIT();
    }
    
    double d = sqrt(OC.dot(OC) - tca*tca);
    
    if(d > r) {
        return Hit::NO_HIT();
    }
    double tch = sqrt(r*r -d*d);
    double t1 = tca-tch;
    double t2 = tca + tch;
    
    
    if (t1 > t2) std::swap(t1,t2);

    

    
    if (t1 < 0) {
        t1 = t2;
        if (t1 < 0) return Hit::NO_HIT(); 
    }

    double t = t1;
    

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    *
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/


    
    Vector p = ray.O + (ray.D*t);
    Vector N = p - position;
    
    N.normalize();
    


    return Hit(t,N);
}

Sphere::Sphere(Point const &pos, double radius)
:
    position(pos),
    r(radius)
{}
