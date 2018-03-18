#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QVector3D>
#include <GL/gl.h>

struct Properties
{
    QVector3D location;
    QVector3D rotation;
    GLuint vaoID;
    GLuint texID;
    float speed;
    int meshSize;

};


#endif // PROPERTIES_H
