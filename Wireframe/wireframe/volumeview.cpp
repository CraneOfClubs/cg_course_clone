#include "volumeview.h"
#include <QMouseEvent>


VolumeView::VolumeView(QWidget *parent) : DrawingArea(parent)
{
    this->setGeometry(40,40,700,500);
    _current_perspective = transform.Perspective(_clip_near, _clip_far, this->width(), this->height());
    _current_rotation = transform.Identity();
    _current_camera = transform.Camera();
}

void VolumeView::drawBoundingBox()
{
    std::vector<Line> boundcube = std::vector<Line>({
                                                    Line(DoublePoint(-1, -1, -1), DoublePoint(1, -1, -1)),
                                                    Line(DoublePoint(-1, -1, -1), DoublePoint(-1, 1, -1)),
                                                    Line(DoublePoint(-1, 1, -1), DoublePoint(1, 1, -1)),
                                                    Line(DoublePoint(1, -1, -1), DoublePoint(1, 1, -1)),
                                                    Line(DoublePoint(-1, -1, 1), DoublePoint(1, -1, 1)),
                                                    Line(DoublePoint(-1, -1, 1), DoublePoint(-1, 1, 1)),
                                                    Line(DoublePoint(-1, 1, 1), DoublePoint(1, 1, 1)),
                                                    Line(DoublePoint(1, -1, 1), DoublePoint(1, 1, 1)),
                                                    Line(DoublePoint(-1, -1, -1), DoublePoint(-1, -1, 1)),
                                                    Line(DoublePoint(-1, 1, -1), DoublePoint(-1, 1, 1)),
                                                    Line(DoublePoint(1, -1, -1), DoublePoint(1, -1, 1)),
                                                    Line(DoublePoint(1, 1, -1), DoublePoint(1, 1, 1)),
                                                   });
    this->drawFile(boundcube, 0x000000FF);
}

void VolumeView::drawFile(std::vector<Line> file, uint32_t color)
{
    DoublePoint from_point;
    DoublePoint to_point;
    for (auto it = file.begin(); it < file.end(); ++it) {
        from_point = it->first();
        to_point = it->second();
        from_point.setCoords(transform.multOnVector(_current_rotation, from_point));
        to_point.setCoords(transform.multOnVector(_current_rotation, to_point));
        from_point.setCoords(transform.multOnVector(_current_camera, from_point));
        to_point.setCoords(transform.multOnVector(_current_camera, to_point));
        from_point.setCoords(transform.multOnVector(_current_perspective, from_point));
        to_point.setCoords(transform.multOnVector(_current_perspective, to_point));

        Line result(from_point, to_point);
        if (result.clipLine()) {
            drawLine(result.firstIn3D() * 100, result.secondIn3D()* 100, color);
        }
    }
}

void VolumeView::drawLine(QVector3D from, QVector3D to,  uint32_t color)
{
    QPoint from_f(qRound(_image->width() / 2 + from.x() / 2 * _image->width()), qRound(_image->height() / 2 - 1 - from.y() / 2 * _image->height()));
    QPoint to_f(qRound(_image->width() / 2 + to.x() / 2 * _image->width()), qRound(_image->height() / 2 - 1 - to.y() / 2 * _image->height()));
    drawLineBresenham(from_f, to_f, color);
}

void VolumeView::redraw() {
    memset(_image->bits(), 255, _image->height() * _image->width() * _image->depth() / 8);
    drawBoundingBox();
    this->update();
}

void VolumeView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        _left_key_pressed = true;
        this->_mouse_start.setX(event->x());
        this->_mouse_start.setY(event->y());
    }
}

void VolumeView::mouseMoveEvent(QMouseEvent *event)
{
    if (_left_key_pressed) {
        float offset_x = (float) (event->x() - _mouse_start.x()) * 2 * pi / _image->width();
        float offset_y = (float) (event->y() - _mouse_start.y()) * 2 * pi / _image->height();
        auto y_transform = transform.RotateY(offset_x);
        auto x_transform = transform.RotateZ(offset_y);
        _current_rotation = this->transform.multOnMatrix(this->transform.multOnMatrix(y_transform, x_transform), _current_rotation);
        this->_mouse_start.setX(event->x());
        this->_mouse_start.setY(event->y());
        this->redraw();
    }
}

void VolumeView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        _left_key_pressed = false;
    }
}

void VolumeView::resizeEvent(QResizeEvent *e)
{
    if (!_image_loaded) {
        delete _image;
        _image = new QImage(QSize(this->width(), this->height()), QImage::Format_RGBA8888);
        memset(_image->bits(), 255, _image->height() * _image->width() * _image->depth() / 8);
        repaint();
    }
    this->redraw();
}
