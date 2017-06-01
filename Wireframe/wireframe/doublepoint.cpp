#include "doublepoint.h"

DoublePoint::DoublePoint()
{

}

DoublePoint::DoublePoint(float x, float y, float z)
{
    _coord.setX(x);
    _coord.setY(y);
    _coord.setZ(z);
    _coord.setW(1.0);
}

QVector4D DoublePoint::get4D()
{
    return this->_coord;
}

QVector3D DoublePoint::castTo3D()
{
    return QVector3D(_coord.x() / _coord.w(), _coord.y() / _coord.w(), _coord.z() / _coord.w());
}


void DoublePoint::scaleToNorm() {
    for (int32_t i = 0; i < 3; ++i) {
        _coord[i] = _coord[i] / _coord[3];
    }
    _coord[3] = 1.0;
}

void DoublePoint::setCoords(QVector4D coordinates) {
    _coord = coordinates;
}



