#ifndef PLANE_H_
#define PLANE_H_

#include "../object.h"

class Plane: public Object
{
    public:
        Plane(Point &pos, Point &n);

        virtual Hit intersect(Ray const &ray);

        Point  position;
        Point  normal;
};

#endif