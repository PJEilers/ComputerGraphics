#include "raytracer.h"

#include "image.h"
#include "light.h"
#include "material.h"
#include "triple.h"
#include "objloader.h"


// =============================================================================
// -- Include all your shapes here ---------------------------------------------
// =============================================================================

#include "shapes/sphere.h"
#include "shapes/plane.h"
#include "shapes/triangle.h"

// =============================================================================
// -- End of shape includes ----------------------------------------------------
// =============================================================================

#include "json/json.h"

#include <utility> // declval, forward, move, pair, swap

#include <exception>
#include <fstream>
#include <iostream>



using namespace std;        // no std:: required
using json = nlohmann::json;

bool Raytracer::parseObjectNode(json const &node)
{
    ObjectPtr obj = nullptr;

// =============================================================================
// -- Determine type and parse object parametrers ------------------------------
// =============================================================================

    if (node["type"] == "sphere")
    {
        Point pos(node["position"]);
        double radius = node["radius"];
        obj = ObjectPtr(new Sphere(pos, radius));
    }
    else if (node["type"] == "plane") 
    {
        Point pos(node["position"]);
        Point normal(node["normal"]);
        obj = ObjectPtr(new Plane(pos, normal));
    }
    else if (node["type"] == "triangle")
    {
        Point v1(node["point1"]);
        Point v2(node["point2"]);
        Point v3(node["point3"]);
        obj = ObjectPtr(new Triangle(v1, v2, v3));
    }
    else if (node["type"] == "mesh") 
    {   
        return initializeMesh(node);
    }
    else
    {
        cerr << "Unknown object type: " << node["type"] << ".\n";
    }

// =============================================================================
// -- End of object reading ----------------------------------------------------
// =============================================================================

    if (!obj)
        return false;

    // Parse material and add object to the scene
    obj->material = parseMaterialNode(node["material"]);
    scene.addObject(obj);
    return true;
}

Light Raytracer::parseLightNode(json const &node) const
{
    Point pos(node["position"]);
    Color col(node["color"]);
    return Light(pos, col);
}

Material Raytracer::parseMaterialNode(json const &node) const
{
    Color color(node["color"]);
    double ka = node["ka"];
    double kd = node["kd"];
    double ks = node["ks"];
    double n  = node["n"];
    return Material(color, ka, kd, ks, n);
}

bool Raytracer::readScene(string const &ifname)
try
{
    // Read and parse input json file
    ifstream infile(ifname);
    if (!infile) throw runtime_error("Could not open input file for reading.");
    json jsonscene;
    infile >> jsonscene;

// =============================================================================
// -- Read your scene data in this section -------------------------------------
// =============================================================================

    Point eye(jsonscene["Eye"]);
    scene.setEye(eye);
    
    //Check if shadows are enabled
    
    json j = jsonscene["Shadows"];
    bool shadows = false;
    if(j.is_boolean()) {
        shadows = j.get<bool>();
    }
    scene.setShadows(shadows);
    
    //Get the max recursion depth for reflections
    j = jsonscene["MaxRecursionDepth"];
    int recursionDepth = 0;
    if(j.is_number()) {
        recursionDepth = j.get<int>();
    }
    scene.setMaxRecursionDepth(recursionDepth);
    //Get the super sampling factor
    j = jsonscene["SuperSamplingFactor"];
    int superSamplingFactor = 1;
    if(j.is_number()) {
        superSamplingFactor = j.get<int>();
    }
    scene.setSuperSamplingFactor(superSamplingFactor);
    
    
    // TODO: add your other configuration settings here

    for (auto const &lightNode : jsonscene["Lights"])
        scene.addLight(parseLightNode(lightNode));

    unsigned objCount = 0;
    for (auto const &objectNode : jsonscene["Objects"])
        if (parseObjectNode(objectNode))
            ++objCount;

    cout << "Parsed " << objCount << " objects.\n";

// =============================================================================
// -- End of scene data reading ------------------------------------------------
// =============================================================================

    return true;
}
catch (exception const &ex)
{
    cerr << ex.what() << '\n';
    return false;
}

void Raytracer::renderToFile(string const &ofname)
{
    // TODO: the size may be a settings in your file
    Image img(400, 400);
    cout << "Tracing...\n";
    scene.render(img);
    cout << "Writing image to " << ofname << "...\n";
    img.write_png(ofname);
    cout << "Done.\n";
}

bool Raytracer::initializeMesh (json const &node) {
    json j = node["name"];
    string name;
        
    if(!j.is_string()) return false;
            
    name = j.get<std::string>(); // Get name
        
    OBJLoader objl(name); // Load object
        
    vector<Vertex> vertices = objl.vertex_data(); //Get vertices of object
        
    json scaleAndOffset = node["scaleoffset"]; //Get scaling factor and offset ratios
        
    if(!scaleAndOffset.is_array()) return false;
        
    double scale = scaleAndOffset[0];
    double offsetX = scaleAndOffset[1];
    double offsetY = scaleAndOffset[2];
    double offsetZ = scaleAndOffset[3];
        
    Material material = parseMaterialNode(node["material"]); //Parse material
        
    /* Adding all triangles in the mesh to the scene*/
        
    for(unsigned int i = 0; i < vertices.size(); i+=3) {            
        Point v1(vertices[i].x*scale+offsetX, vertices[i].y*scale+offsetY, vertices[i].z*scale+offsetZ);
        Point v2(vertices[i+1].x*scale+offsetX , vertices[i+1].y*scale+offsetY, vertices[i+1].z*scale+offsetZ);
        Point v3(vertices[i+2].x*scale+offsetX , vertices[i+2].y*scale+offsetY, vertices[i+2].z*scale+offsetZ);
        ObjectPtr obj = ObjectPtr(new Triangle(v1,v2,v3));
        obj->material = material;
        scene.addObject(obj);
    }
    return true;
}
