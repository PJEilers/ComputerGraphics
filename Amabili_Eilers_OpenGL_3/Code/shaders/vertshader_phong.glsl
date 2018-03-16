#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormals_in;
layout (location = 2) in vec2 textureCoordinates_in;

// Specify the Uniforms of the vertex shader
// uniform mat4 modelTransform; for example

uniform mat4 modelViewTransform;
uniform mat4 projectionTransform;
uniform mat3 normalMatrix;

// Specify the output of the vertex stage
out vec3 vertNormals;
out vec3 position;
out vec2 textureCoordinates;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    // Currently without any transformation
    position = vec3(modelViewTransform * vec4(vertCoordinates_in, 1.0));
    gl_Position =  projectionTransform *  modelViewTransform  * vec4(vertCoordinates_in, 1.0);
    vertNormals = normalMatrix*vertNormals_in;
    textureCoordinates = textureCoordinates_in;
    //vertNormals = vertNormals_in;
}
