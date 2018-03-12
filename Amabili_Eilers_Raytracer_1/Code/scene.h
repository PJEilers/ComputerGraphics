#ifndef SCENE_H_
#define SCENE_H_

#include "light.h"
#include "object.h"
#include "triple.h"
#include "material.h"

#include <vector>

// Forward declerations
class Ray;
class Image;

class Scene
{
    std::vector<ObjectPtr> objects;
    std::vector<LightPtr> lights;   // no ptr needed, but kept for consistency
    Point eye;
    bool shadows;
    int maxRecursionDepth;
    int superSamplingFactor;

    public:

        // trace a ray into the scene and return the color
        Color trace(Ray const &ray);
        Color getDiffuseAndSpecularLighting(Material material, Point hit, Vector N, Vector V, LightPtr light, ObjectPtr obj);
        Color getSpecularReflection(Material material, Ray r, Vector N, double ks, Color reflected, int depth);

        // render the scene to the given image
        void render(Image &img);


        void addObject(ObjectPtr obj);
        void addLight(Light const &light);
        void setEye(Triple const &position);
        void setShadows(bool s);
        void setMaxRecursionDepth(int depth);
        void setSuperSamplingFactor(int factor);
 
        
        unsigned getNumObject();
        unsigned getNumLights();
};

#endif
