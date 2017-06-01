#include "bsplineview.h"
#include <QPoint>

BsplineView::BsplineView(QWidget *parent) : DrawingArea(parent)
{
    this->setGeometry(40,40,700,500);
}

void BsplineView::loadObject(CurvedObject* object)
{
    delete _image;
    _image = new QImage(QSize(this->width(), this->height()), QImage::Format_RGBA8888);
    memset(_image->bits(), 255, _image->height() * _image->width() * _image->depth() / 8);
    _image_loaded = true;
    _object = *object;
    drawObject();
}

float BsplineView::detectScale(std::vector<QPointF> &knots)
{
    float x_max = knots[0].x();
    float y_max = knots[0].y();
    for (int32_t i = 0; i < knots.size(); i++) {
        if (std::fabs(x_max) < std::fabs(knots[i].x())) {
            x_max =knots[i].x();
        }
    }
    for (int32_t i = 0; i < knots.size(); i++) {
        if (std::fabs(y_max) < std::fabs(knots[i].y())) {
            y_max = knots[i].y();
        }
    }
    if (x_max == 0) x_max = 1;
    if (y_max == 0) y_max = 1;
    return std::min((float) this->width() / (x_max * 2), (float) this->height() / (y_max * 2));
}

void BsplineView::drawObject()
{
    fill(0x111111FF);
    QPoint start = QPoint(std::round((float) this->width() / 2), 0);
    QPoint end = QPoint(std::round((float) this->width() / 2), this->height() - 1);
    drawLineBresenham(start, end, 0xFF00FFFF);
    start = QPoint(0, std::round((float) this->height() / 2));
    end = QPoint(this->width() - 1, std::round((float) this->height() / 2));
    drawLineBresenham(start, end, 0xFF00FFFF);
    auto knots = this->_object.getKnots();
    float scale = this->detectScale(knots);
    int x_offset = std::round((float) this->width() / 2);
    int y_offset = std::round((float) this->height() / 2);
    scale *= def_scale;
    if (knots.size() > 0) {
        QPointF prev = knots[0];
        for (auto it = knots.begin() + 1; it < knots.end(); ++it) {
            QPoint f_s_knot = QPoint(scale * prev.x() + x_offset, scale * prev.y() + y_offset);
            QPoint f_e_knot = QPoint(scale * it->x() + x_offset, scale * it->y() + y_offset);
            drawLineBresenham(f_s_knot, f_e_knot, 0xFFFF00FF);
            prev = *it;
        }
    }
    auto segments = this->_object.getSegmentsPlain();
    for (auto it = segments.begin(); it < segments.end(); ++it) {
        QPoint f_s_seg = QPoint(scale * it->first.x() + x_offset, scale * it->first.y() + y_offset);
        QPoint f_e_seg = QPoint(scale * it->second.x() + x_offset, scale * it->second.y() + y_offset);
        drawLineBresenham(f_s_seg, f_e_seg, 0x00FF00FF);
    }
    this->repaint();
}
