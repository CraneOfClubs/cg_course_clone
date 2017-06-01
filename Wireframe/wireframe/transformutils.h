#ifndef TRANSFORMUTILS_H
#define TRANSFORMUTILS_H
#include <QMatrix4x4>
#include <QVector4D>
#include <doublepoint.h>

class TransformUtils
{
private:

public:
    TransformUtils();
    QVector4D multOnVector(QMatrix4x4 matrix, DoublePoint point);
    QMatrix4x4 multOnMatrix(QMatrix4x4 first, QMatrix4x4 second);
    QMatrix4x4 Identity();
    QMatrix4x4 RotateX(float angle);
    QMatrix4x4 RotateY(float angle);
    QMatrix4x4 RotateZ(float angle);
    QMatrix4x4 Perspective(float zn, float zf, float sw, float sh);
    QMatrix4x4 Camera();
};

#endif // TRANSFORMUTILS_H
