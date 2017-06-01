#include "line.h"

Line::Line(DoublePoint from, DoublePoint to)
{
    _first = from;
    _second = to;
}

QVector3D Line::firstIn3D()
{
    return _first.castTo3D();
}

QVector3D Line::secondIn3D()
{
    return _second.castTo3D();
}

DoublePoint Line::first()
{
    return _first;
}

DoublePoint Line::second()
{
    return _second;
}


bool Line::clipLine()
{
    _first.scaleToNorm();
    _second.scaleToNorm();
    return clipLine(0, -1, 1) && clipLine(1, -1, 1) && clipLine(2, 0, 1);
}

bool Line::clipLine(int position, float min, float max)
{
    float from = _first.get4D()[position];
    float to = _second.get4D()[position];
    if ((from < min && to < min) || (from > max && to > max)) {
        return false;
    }
    if (from > to) {
        std::swap(_first, _second);
        std::swap(from, to);
    }
    if (to > max) {
        QVector3D diff(_second.get4D().x() - _first.get4D().x(), _second.get4D().y() - _first.get4D().y(), _second.get4D().z() - _first.get4D().z());
        diff *= ((max - from) / (to - from));
        for (int i = 0; i < 3; ++i) {
            _second.get4D()[i] = diff[i] + _first.get4D()[i];
        }
        _second.get4D()[3] = 1;
    }
    if (from < min) {
        QVector3D diff(_second.get4D().x() - _first.get4D().x(), _second.get4D().y() - _first.get4D().y(), _second.get4D().z() - _first.get4D().z());
        diff *= ((to - min) / (to - from));
        for (int i = 0; i < 3; ++i) {
            _first.get4D()[i] = _second.get4D()[i] - diff[i];
        }
        _first.get4D()[3] = 1;
    }

    return true;
}
