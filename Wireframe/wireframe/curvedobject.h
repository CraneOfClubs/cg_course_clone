#ifndef CURVEDOBJECT_H
#define CURVEDOBJECT_H
#include <QPoint>
#include <QMatrix4x4>
#include <bspline.h>
#include <line.h>


class CurvedObject
{
public:
    CurvedObject();
    CurvedObject(int n, int m, int k, float a, float b, float c, float d, std::vector<QPointF> knots, uint32_t color);
    void buildSegments();
    std::vector<QPointF> getKnots();
    const std::vector<Line> getSegments();
    std::vector<std::pair<QPointF, QPointF> > getSegmentsPlain();
private:
    uint32_t _color;
    QMatrix4x4 _rotation_matrix;
    std::vector<QPointF> _knots;
    std::vector<Line> _segments;
    BSpline _spline;
    float _a;
    float _b;
    float _c;
    float _d;
    int _n;
    int _m;
    int _k;
};

#endif // CURVEDOBJECT_H
