#include "properties.h"

void setProperties(QVector3D location, QVector3D rotation, float speed, int meshSize, Properties &p) {
    p.location = location;
    p.rotation = rotation;
    p.speed = speed;
    p.meshSize = meshSize;
}
