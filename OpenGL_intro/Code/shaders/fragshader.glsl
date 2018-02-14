#version 330 core


out vec4 rgba;

in vec3 interpolatedColor;
void main()
{
    rgba = vec4(interpolatedColor, 1);
}
