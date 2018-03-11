#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "triple.h"

class Material
{
    public:
        Color color;        // base color
        double ka;          // ambient intensity
        double kd;          // diffuse intensity
        double ks;          // specular intensity
        double n;           // exponent for specular highlight size

        Material() = default;

        Material(Color const &color, double ka, double kd, double ks, double n)
        :
            color(color),
            ka(ka),
            kd(kd),
            ks(ks),
            n(n)
        {}
};

#endif
