#ifndef BSPLINE_H
#define BSPLINE_H
#include <matrix.h>
#include <doublepoint.h>
#include <QMatrix4x4>
#include <QVector4D>
#include <QPoint>

class BSpline
{
public:
    BSpline();
    BSpline(std::vector<QPointF> values);
    QPointF calculate(double t);
    QPointF calculate(int knot, double t);
private:
    float getLength();
    std::pair<int, float> getKnotByLength(float length_scale);
    void calcCoofOfMatr();
    QMatrix4x4 spline_matrix;
    std::vector<QPointF> values;
    std::vector<QVector4D> vectors_x;
    std::vector<QVector4D> vectors_y;
};

#endif // BSPLINE_H
