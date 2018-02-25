#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../object.h"

class Triangle: public Object
{
    public:
        Triangle(Point const &v1, Point const &v2, Point const &v3);

        virtual Hit intersect(Ray const &ray);

        /* My data members*/

        Point const v1;
        Point const v2;
        Point const v3;

};

#endif
