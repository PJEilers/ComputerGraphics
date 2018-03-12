#ifndef SPHERE_H_
#define SPHERE_H_

#include "../object.h"
#include "../image.h"

class Sphere: public Object
{
    public:
        Sphere(Point const &pos, double radius);
        
        Sphere(Point const &pos, double radius, Image texture);

        virtual Hit intersect(Ray const &ray);
        
        virtual Color getTextureColor(Point hit);

        Point const position;
        double const r;
        Image texture;
};

#endif
