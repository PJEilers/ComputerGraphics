#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormals_in;

// Specify the Uniforms of the vertex shader
// uniform mat4 modelTransform; for example

uniform mat4 modelTransform;
uniform mat4 projectTransform;
uniform mat3 normalMatrix;

uniform vec3 lightPosition;
uniform vec3 material;



// Specify the output of the vertex stage
out vec3 vertNormals;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    // Currently without any transformation

    // Transform the vertex into eye space.
    vec3 modelViewVertex = vec3(modelTransform * vertCoordinates_in);

    // Transform the normal's orientation into eye space.
    vec3 modelViewNormal = vec3(modelTransform * vec4(vertNormals, 0.0));

    // Get a lighting direction vector from the light to the vertex.
    vec3 lightVector = normalize(lightPosition - modelViewVertex);

    // Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
    // pointing in the same direction then it will get max illumination.
    float diffuse = max(dot(modelViewNormal, lightVector), 0.01);

    // Multiply the color by the illumination level. It will be interpolated across the triangle.
    vertNormals = vertNormals_in * diffuse;

    gl_Position =  projectTransform *  modelTransform  * vec4(vertCoordinates_in, 1.0);

   // vertNormals = normalMatrix*vertNormals_in;
    //vertNormals = vertNormals_in;
}
