#ifndef LINE_H
#define LINE_H
#include <doublepoint.h>

class Line
{
public:
    Line();
    Line(DoublePoint first, DoublePoint second);
    DoublePoint first();
    DoublePoint second();
    QVector3D secondIn3D();
    QVector3D firstIn3D();
    bool clipLine();
private:
    bool clipLine(int position, float min, float max);

    DoublePoint _first;
    DoublePoint _second;
};

#endif // LINE_H
