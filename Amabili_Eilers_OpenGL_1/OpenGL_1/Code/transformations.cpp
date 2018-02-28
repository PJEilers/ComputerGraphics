#include "transformations.h"

void setRotate(float x, float y, float z, transformations &t) {
    t.rotateX = x;
    t.rotateY = y;
    t.rotateZ = z;
}

void setTranslation(float x, float y, float z, transformations &t) {
    t.translateX = x;
    t.translateY = y;
    t.translateZ = z;
}

void setScale(float s, transformations &t) {
    t.scale = s;
}


