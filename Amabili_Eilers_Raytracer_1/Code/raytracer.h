#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "scene.h"

#include <string>

// Forward declerations
class Light;
class Material;

#include "json/json_fwd.h"

class Raytracer
{
    Scene scene;

    public:

        bool readScene(std::string const &ifname);
        void renderToFile(std::string const &ofname);

    private:

        bool parseObjectNode(nlohmann::json &node);
        bool initializeMesh(nlohmann::json &node);
        Light parseLightNode(nlohmann::json &node) const;
        Material parseMaterialNode(nlohmann::json &node) const;
};

#endif
