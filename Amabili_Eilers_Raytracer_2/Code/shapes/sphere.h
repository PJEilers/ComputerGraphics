#ifndef SPHERE_H_
#define SPHERE_H_

#include "../object.h"

class Sphere: public Object
{
    public:
        Sphere(Point const &pos, double radius);

        virtual Hit intersect(Ray const &ray);
        
        double map(Point const &pos);
        
        Point const position;
        double const r;
};

#endif
