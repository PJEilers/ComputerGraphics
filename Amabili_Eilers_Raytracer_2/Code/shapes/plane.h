#ifndef PLANE_H_
#define PLANE_H_

#include "../object.h"

/**
 * Plane is defined by its position and its normal defining the orientation of the plane
 */

class Plane: public Object
{
    public:
        Plane(const Point &pos, const Point &n);

        virtual Hit intersect(Ray const &ray);

        const Point position;
        const Point normal;
};

#endif