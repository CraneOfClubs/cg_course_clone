#include "bspline.h"
#include <QVector4D>

BSpline::BSpline()
{

}

BSpline::BSpline(std::vector<QPointF> values): vectors_x(values.size()), vectors_y(values.size())
{
    this->values = values;
    spline_matrix = QMatrix4x4(-1.0/6.0,  3.0/6.0, -3.0/6.0, 1.0/6.0,
                         3.0/6.0, -6.0/6.0,  3.0/6.0, 0.0/6.0,
                        -3.0/6.0,  0.0/6.0,  3.0/6.0, 0.0/6.0,
                         1.0/6.0,  4.0/6.0,  1.0/6.0, 0.0/6.0);
    this->calcCoofOfMatr();
}

QPointF BSpline::calculate(double len)
{
    auto pair = this->getKnotByLength(len);
    return calculate(pair.first, len);
}

QPointF BSpline::calculate(int knot, double len)
{
    QVector4D transf_vect(std::pow(len, 3), std::pow(len, 2), len, 1);
    QPointF point;
    point.setX(QVector4D::dotProduct(transf_vect, this->vectors_x[knot]));
    point.setY(QVector4D::dotProduct(transf_vect, this->vectors_y[knot]));
    return point;
}


std::pair<int, float> BSpline::getKnotByLength(float length_scale)
{
    float length = this->getLength() * length_scale;

    float curr_len = 0;
    for (uint i = 1; i < this->values.size() - 2; ++i) {
        QPointF prev = calculate(i, 0);
        for (float t = 0.001f; t < 1.0f; t += 0.001f) {
            QPointF curr = calculate(i, t);
            curr_len += std::sqrt(std::pow(curr.x() - prev.x(), 2) + std::pow(curr.y() - prev.y(), 2));
            if (curr_len > length) {
                return std::make_pair(i, t);
            }
            prev = curr;
        }
    }
    return std::make_pair(this->values.size() - 2, 0);
}



void BSpline::calcCoofOfMatr()
{
    for (uint i = 1; i < this->values.size(); ++i) {
        QVector4D x(this->values[i - 1].x(), this->values[i].x(), this->values[i + 1].x(), this->values[i + 2].x());
        QVector4D y(this->values[i - 1].y(), this->values[i].y(), this->values[i + 1].y(), this->values[i + 2].y());
        this->vectors_x[i] = this->spline_matrix * x;
        this->vectors_y[i] = this->spline_matrix * y;
    }
}

float BSpline::getLength()
{
    float len = 0;
    for (uint i = 1; i < this->values.size() - 2; ++i) {
        QPointF previous = calculate(i, 0);
        for (float f_j = 0.001f; f_j < 1.0f; f_j += 0.001f) {
            QPointF currentdelt = calculate(i, f_j);
            len += std::sqrt(std::pow(currentdelt.x() - previous.x(), 2) + std::pow(currentdelt.y() - previous.y(), 2));
        }
    }
    return len;
}


//    spline_matrix = QMatrix4x4(1.0/2.0,  1.0/2.0, 0.0/2.0, 0.0/2.0,
//                         -2.0/2.0, 2.0/2.0,  0.0/2.0, 0.0/2.0,
//                        -3.0/2.0,  0.0/2.0,  3.0/2.0, 0.0/2.0,
//                         1.0/2.0,  -2.0/2.0,  1.0/2.0, 0.0/2.0);
