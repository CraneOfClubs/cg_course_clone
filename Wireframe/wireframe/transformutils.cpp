#include "transformutils.h"

TransformUtils::TransformUtils()
{

}

QVector4D TransformUtils::multOnVector(QMatrix4x4 matrix, DoublePoint point)
{
    return matrix * point.get4D();
}

QMatrix4x4 TransformUtils::multOnMatrix(QMatrix4x4 first, QMatrix4x4 second)
{
    return first * second;
}

QMatrix4x4 TransformUtils::Identity()
{
    return {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
}

QMatrix4x4 TransformUtils::RotateX(float angle)
{
    float cos_angle = std::cos(angle);
    float sin_angle = std::sin(angle);
    return {1.0, 0.0, 0.0, 0.0, 0.0, cos_angle, -sin_angle, 0.0, 0.0, sin_angle,  cos_angle, 0.0, 0.0, 0.0, 0.0, 1.0};
}

QMatrix4x4 TransformUtils::RotateY(float angle)
{
    float cos_angle = std::cos(angle);
    float sin_angle = std::sin(angle);
    return {cos_angle,  0.0, sin_angle, 0.0, 0.0, 1.0, 0.0, 0.0, -sin_angle, 0.0, cos_angle, 0.0, 0.0, 0.0, 0.0, 1.0};
}

QMatrix4x4 TransformUtils::RotateZ(float angle)
{
    float cos_angle = std::cos(angle);
    float sin_angle = std::sin(angle);
    return {cos_angle, -sin_angle, 0.0, 0.0,sin_angle,  cos_angle, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
}


QMatrix4x4 TransformUtils::Perspective(float zn, float zf, float sw, float sh)
{
    return {2 * zf / sw, 0, 0, 0, 0, 2 * zf / sh, 0, 0, 0, 0, zf / (zn - zf), zn * zf / (zn - zf), 0, 0, -1, 0};
}

QMatrix4x4 TransformUtils::Camera()
{
    QVector3D normal(0, 1, 0);
    QVector3D reference(7, 0, 0);
    QVector3D eye(-5, 0, 0);
    QVector3D w(eye - reference);
    QVector3D u(QVector3D::crossProduct(normal, w));
    w.normalize();
    u.normalize();
    QVector3D v(QVector3D::crossProduct(w, u));
    auto rot = QMatrix4x4(u[0], u[1], u[2], 0, v[0], v[1], v[2], 0, w[0], w[1], w[2], 0, 0, 0, 0, 1);
    auto move = QMatrix4x4(1, 0, 0, -reference[0], 0, 1, 0, -reference[1], 0, 0, 1, -reference[2], 0, 0, 0, 1);
    return rot * move;
}
