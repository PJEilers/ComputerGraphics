### Assignment #4 - Raytracer 2 - Shaders and Texture Mapping - Computer Graphics course - 
made by Lorenzo Amabili(p281542) and Pieter Jan Eilers(s2381575).

## The project consisted of implementing a set of shading techniques (i.e., Normal shader, Gouraud shader, and Phong shader) in such a way that users can swap between them. The work is built upon the previous software created for the first assignment. We decided to continue building on the starting point for this assigment that was given on nestor since our Open_GL_1 results were not very good.
#Testing the implementation of the Normal shader, we noticed that without normalizing the normal at each vertex in the vertex shader the final result is biased. The colors of each side of the cat are always the same inpendently by the transformations applied. This does not occur when using the normal matrix.
##The second part consisted of mapping a square diffuse texture to a mesh. We used the given mesh of a cat and the gives texture cat_diff.png that goes with it. If we change the wrap by multiplying the coordinates in the vertshader by (2,2) for example, the texture does not really fit anymore and the result is messed up. See screenshots.
Change the filtering from GL_NEAREST to GL_LINEAR did not seem to change anything, also mipmapping didn't seem to change anything, not even on extreme zooming, but maybe we were using it incorrectly.

## Following the instructions, we completed successfully the implementation of "Shaders" (i.e., (a) Normal Shader (b) Gouraud Shader  (c) Phong Shader), and of "Texture mapping" (i.e., 

## Prerequisites
: cmake, building still works the same, we did not add multithreading.
 
## We used a GitHub repository for our project: https://github.com/PJEilers/ComputerGraphics


## The project consisted of producing a 3D scene using a basic ray tracing algorithm, and the implementation of (Phong) illumination will be based on the intersection calculation together with normal calculation. Additional geometries such as a triangle and a plane have been implemented too. Also the ability to load in an object file containing a mesh and visualizing that mesh using raytracing
## Following the instructions, we completed successfully the implementation of "Raycasting with spheres & Phong illumination" (i.e., (a) Phong diffuse (b) Full Phong (c) Scene 2) ), whereas we partially completed the section "Additional geometry and meshes ".
## Including is a folder "Scenes" with two scenes in the form of two json files and two object files so one can recreate the results.
