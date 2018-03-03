#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

struct Vertex
{
    QVector3D coords;
    QVector3D color;

    Vertex(QVector3D coords, QVector3D color)
        :
          coords(coords),
          color(color)
    {    }
};

#endif // VERTEX_H

