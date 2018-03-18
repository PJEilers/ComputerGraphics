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
    double speed;
    int meshSize;

};

void setProperties(QVector3D location, QVector3D rotation, double speed, int meshSize, Properties &p);


#endif // PROPERTIES_H
