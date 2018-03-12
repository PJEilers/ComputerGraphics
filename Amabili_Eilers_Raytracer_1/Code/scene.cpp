#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <cmath>
#include <limits>

#define BIAS 1e-5

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
    Vector R = N*2*V.dot(N) - V;                   //Reflection vector
    Ray reflectionRay(hit + N*BIAS, R);            //Reflection ray
    
    if(material.hasTexture) 
    
    if(shadows) {
        Color color = material.color*material.ka;
        
        //Checking if the intersection point is in the shadows of another object
        
        for(auto light : lights) {
            Hit min_hit(numeric_limits<double>::infinity(), Vector());
            bool shadowed = false;
            Vector L = (light->position - hit).normalized();
            Ray r(hit + N*BIAS, L);
            for (unsigned idx = 0; idx != objects.size(); ++idx)
            {
                Hit h(objects[idx]->intersect(r));
                if (h.t < min_hit.t)
                {
                    shadowed = true;
                    break;
                } 
                
            }
            if(!shadowed) color += getDiffuseAndSpecularLighting(material, hit, N, V, light, obj);
        }
        
        color += getSpecularReflection(material,reflectionRay,N,material.ks, Color(0.0,0.0,0.0), 0);
        return color;
    }
    
    //Get full phong lighting
    Color color(0.0,0.0,0.0);
        
    for (auto light : lights) {
        color += getDiffuseAndSpecularLighting(material, hit, N, V, light, obj);
        color += getSpecularReflection(material, reflectionRay, N, material.ks , Color(0.0,0.0,0.0) , 0);
    }

    

    color += material.color*material.ka;

  
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
            
            //Super sampling, Standard super sampling factor is 1
            
            Color col(0.0,0.0,0.0);
            double ssFactor = (double) superSamplingFactor;
            
            for(double i = 0; i < ssFactor; i++) {
                double yCoord = h - 1 - y +  ((1.0+2.0*i)/(ssFactor*2.0));
                for(double j = 0; j < superSamplingFactor; j++) {
                    double xCoord = x + (double) ((1.0+2.0*j)/(ssFactor*2.0));
                    Point pixel (xCoord, yCoord);
                    Ray ray(eye, (pixel - eye).normalized());
                    col += trace(ray);
                }
            }
            
            col = col/(superSamplingFactor*superSamplingFactor);
            col.clamp();
            img(x,y) = col;
            
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

void Scene::setShadows(bool s) {
    shadows = s;
}

void Scene::setMaxRecursionDepth(int depth) {
    maxRecursionDepth = depth;
}

void Scene::setSuperSamplingFactor(int factor) { 
    superSamplingFactor = factor;
}

/**
 * @brief Calculates diffuse and specular lighting
 * @param Material of the shape, point of intersection, normal vector, view vector
 * @returns The color of the pixel the be drawn
 */

Color Scene::getDiffuseAndSpecularLighting(Material material, Point hit, Vector N, Vector V, LightPtr light, ObjectPtr obj) {
    
       
    Color materialColor;
    if(material.hasTexture) {
        obj->getTextureColor(hit);
    }
    
    Color color(0.0, 0.0, 0.0);
    Vector L = (light->position - hit).normalized(); 
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
    
    return color;
}

/**
 * @brief Recursively calculates the color of the specular reflection.
 * @param Material of the shape, reflection ray, normal vector, specular coefficient, color of reflection, current depth 
 * @returns The reflection color
 */


Color Scene::getSpecularReflection(Material material, Ray r, Vector N, double ks, Color reflected, int depth) {
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    
    if(ks == 0.0) return reflected; //Material is not shiny
    if(depth == maxRecursionDepth) return reflected;
    
    ObjectPtr obj = nullptr;
    for (unsigned idx = 0; idx != objects.size(); ++idx)
    {
        Hit h(objects[idx]->intersect(r));
        if (h.t < min_hit.t)
        {
            min_hit = h;
            obj = objects[idx];
        }
    }
    
    if(!obj) return reflected; //No hit
    Point hitPoint = r.at(min_hit.t);
    
    Material newMaterial = obj->material;
 
    
    reflected += newMaterial.color * ks;
    
    N = min_hit.N;
        
    Vector V = -r.D;
    Vector R = N * 2 * V.dot(N) -  V; //New reflection vector
    Ray newRay(hitPoint+N*BIAS, R); //New reflection ray
    return getSpecularReflection(newMaterial, newRay, N, ks * newMaterial.ks, reflected, depth+1 );    
}




