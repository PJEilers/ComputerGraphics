#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../object.h"

/**
 * A triangle is consists of three points or vertices in 3d space.
 */

class Triangle: public Object
{
    public:
        Triangle(Point const &v1, Point const &v2, Point const &v3);

        virtual Hit intersect(Ray const &ray);

        /* Data members*/

        Point const v1;
        Point const v2;
        Point const v3;

};

#endif
