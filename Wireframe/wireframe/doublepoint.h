#ifndef DOUBLEPOINT_H
#define DOUBLEPOINT_H
#include <QVector4D>
#include <QVector3D>

class DoublePoint
{
private:
    QVector4D _coord;
public:

    DoublePoint(float x, float y, float z);
    QVector3D castTo3D();
    QVector4D get4D();
    void scaleToNorm();
    DoublePoint();
    void setCoords(QVector4D coordinates);
};

#endif // DOUBLEPOINT_H
