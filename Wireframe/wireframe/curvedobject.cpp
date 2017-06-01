#include "curvedobject.h"

CurvedObject::CurvedObject()
{

}

CurvedObject::CurvedObject(int n, int m, int k, float a, float b, float c, float d, std::vector<QPointF> knots, uint32_t color)
{
    _n = n;
    _m = m;
    _k = k;
    _a = a;
    _b = b;
    _c = c;
    _d = d;
    _knots = knots;
    _color = color;
    _spline = BSpline(knots);
    this->buildSegments();
}

std::vector<QPointF> CurvedObject::getKnots()
{
    return _knots;
}

const std::vector<Line> CurvedObject::getSegments()
{
    return _segments;
}

void CurvedObject::buildSegments()
{
    float long_step = (_b - _a) / (_n);
    float rotate_step = (_d - _c) / (_m);
    for (float l = _a; l < _b; l += long_step) {
        for (float i = _c; i <= _d; i += rotate_step) {
            QPointF point = this->_spline.calculate(l);
            if (i + rotate_step <= _d) {
                DoublePoint start_rotate(point.y() * std::cos(i), point.y() * std::sin(i), point.x());
                for (int j = 1; j <= _k; ++j) {
                    DoublePoint end_rotate(point.y() * std::cos(i + j * rotate_step / _k),point.y() * std::sin(i + j * rotate_step / _k),point.x());
                    this->_segments.push_back(Line(start_rotate, end_rotate));
                    start_rotate = end_rotate;
                }
            }
            if (l + long_step <= _b) {
                DoublePoint start_long(point.y() * std::cos(i), point.y() * std::sin(i), point.x());

                for (int j = 1; j <= _k; ++j) {
                    point = this->_spline.calculate(l + j * long_step / _k);
                    DoublePoint end_long(point.y() * std::cos(i), point.y() * std::sin(i), point.x());
                    this->_segments.push_back(Line(start_long, end_long));
                    start_long = end_long;
                }
            }
        }
    }
}

std::vector<std::pair<QPointF, QPointF>> CurvedObject::getSegmentsPlain()
{
    std::vector<std::pair<QPointF, QPointF>> segments;
    uint count = _k * _n;
    QPointF from = this->_spline.calculate(0);
    for (uint j = 1; j <= count; ++j) {
        QPointF to = this->_spline.calculate((float) j / count);
        segments.push_back(std::make_pair(from, to));
        from = to;
    }
    return segments;
}
