#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormals_in;

// Specify the Uniforms of the vertex shader
// uniform mat4 modelTransform; for example

uniform mat4 modelViewTransform;
uniform mat4 projectionTransform;
uniform mat3 normalMatrix;

uniform vec3 lightPosition;
uniform vec3 materialColor;
uniform vec4 materialStats;
// Specify the output of the vertex stage
out vec3 vertNormals;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    // Currently without any transformation

    // Transform the vertex into eye space.
    vec3 modelViewVertex = vec3(modelViewTransform * vec4(vertCoordinates_in,1.0));

    // Transform the normal's orientation into eye space.
    vec3 n = normalize(vec3(normalMatrix * vertNormals_in));


    // Get a lighting direction vector from the light to the vertex.
    vec3 lightVector = normalize(lightPosition - modelViewVertex);
    // Get view vector
    vec3 view = normalize(-vertCoordinates_in.xyz);
    // Get reflection vector
    vec3 reflected = reflect(-lightVector, n);

    // Calculate the dot product of the light vector and vertex normal.
    float dotP = max(dot(n, lightVector), 0.0);

    //diffuse
    vec3 diffuse = materialColor * materialStats.y * dotP;

    //specular
    vec3 specular = vec3(0.0);
    if(dotP > 0.0) specular = materialColor * materialStats.z * pow(max(dot(reflected, view), 0.0), materialStats.w);

    // Multiply the color by the illumination level. It will be interpolated across the triangle.

    vec3 ambient = materialColor * materialStats.x;
    vertNormals = diffuse + specular + ambient;

    gl_Position =  projectionTransform *  modelViewTransform  * vec4(vertCoordinates_in, 1.0);

}
