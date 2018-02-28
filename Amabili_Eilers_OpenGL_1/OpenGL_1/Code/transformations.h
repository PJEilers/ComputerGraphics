#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

struct transformations {
    float rotateX;
    float rotateY;
    float rotateZ;
    float translateX;
    float translateY;
    float translateZ;
    float scale;
};

void setRotate(float x, float y, float z, transformations &t);
void setTranslation(float x, float y, float z, transformations &t);
void reset(transformations &t);


#endif // TRANSFORMATIONS_H

