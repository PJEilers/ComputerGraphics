#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!
in vec3 vertNormals;
in vec3 position;
in vec2 textureCoordinates;

// Specify the Uniforms of the fragment shaders
uniform vec3 lightPosition;
uniform vec3 materialColor;
uniform vec4 materialStats;
uniform sampler2D s2d;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 textureColor;

void main()
{

    vec3 n = normalize(vertNormals);

    // Get a lighting direction vector from the light to the fragment
    vec3 lightVector = normalize(lightPosition - position);
    // Get view vector
    vec3 view = normalize(-position);
    // Get reflection vector
    vec3 reflected = reflect(-lightVector, n);

    // Calculate the dot product of the light vector and vertex normal.
    float dotP = max(dot(n, lightVector), 0.0);

    //diffuse
    float diffuse = materialStats.y * dotP;

    //specular
    float specular = 0;
    if(dotP > 0.0) specular = materialStats.z * pow(max(dot(reflected, view), 0.0), materialStats.w);

    //Ambient
    float ambient = materialStats.x;

    float phong = ambient + specular + diffuse;

    //Full Phong
    textureColor = texture2D(s2d, textureCoordinates)*phong;
}
