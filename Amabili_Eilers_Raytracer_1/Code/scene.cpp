#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <cmath>
#include <limits>

using namespace std;

Color Scene::trace(Ray const &ray)
{
    // Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr obj = nullptr;
    for (unsigned idx = 0; idx != objects.size(); ++idx)
    {
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            obj = objects[idx];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material material = obj->material;             //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //View direction 
    
    Color color = getLighting(material, hit, N, V); //Get full phong lighting

    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector + Vector    vector sum
    *        Vector - Vector    vector difference
    *        Point - Point      yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double * Color     scales each color component (r,g,b)
    *        Color * Color      dito
    *        pow(a,b)           a to the power of b
    ****************************************************/

  
    return color;
}

void Scene::render(Image &img)
{
    unsigned w = img.width();
    unsigned h = img.height();
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
            Point pixel(x + 0.5, h - 1 - y + 0.5, 0);
            Ray ray(eye, (pixel - eye).normalized());
            Color col = trace(ray);
            col.clamp();
            img(x, y) = col;
        }
    }
}

// --- Misc functions ----------------------------------------------------------

void Scene::addObject(ObjectPtr obj)
{
    objects.push_back(obj);
}

void Scene::addLight(Light const &light)
{
    lights.push_back(LightPtr(new Light(light)));
}

void Scene::setEye(Triple const &position)
{
    eye = position;
}

unsigned Scene::getNumObject()
{
    return objects.size();
}

unsigned Scene::getNumLights()
{
    return lights.size();
}

/**
 * @brief Calculates full phong lighting
 * @param Material of the shape, point of intersection, normal vector, view vector
 * @returns The color of the pixel the be drawn
 */

Color Scene::getLighting(Material material, Point hit, Vector N, Vector V) {
    
    Color color(0.0, 0.0, 0.0);
    for(LightPtr &light : lights) {
        Vector L = light->position - hit; 
        L.normalize();
        Vector R = N * 2 * L.dot(N) - L;
    
        //Diffuse
    
        Color id(0.0,0.0,0.0);
        double dot = L.dot(N);
        if(dot > 0) id = dot *  material.color* light->color * material.kd;
    
        //Specular
    
        dot = R.dot(V);    
        Color is(0.0,0.0,0.0);
        if(dot > 0) is = pow(dot, material.n) * light->color*material.ks;
    
        color = color + id + is;
    }
    
    //Ambient
    
    Color ia = material.color * material.ka;
    
    return color+ia;
}
