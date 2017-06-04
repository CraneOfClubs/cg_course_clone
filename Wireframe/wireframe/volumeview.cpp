#include "volumeview.h"
#include <QMouseEvent>


VolumeView::VolumeView(QWidget *parent) : DrawingArea(parent)
{
    this->setGeometry(40,40,700,500);
    _current_perspective = transform.Perspective(_clip_near, _clip_far, this->width(), this->height());
    _current_rotation = transform.Identity();
    _current_camera = transform.Camera();
}

void VolumeView::loadObject(CurvedObject obj) {
    _object = obj;
    _objectloaded = true;
}

void VolumeView::loadObjects(std::vector<CurvedObject> objects) {
    _objects = objects;
    _objectloaded = true;
    this->redraw();
}

void VolumeView::drawBoundingBox(float coord)
{
    coord *= 1.2;
    std::vector<Line> boundcube = std::vector<Line>({
                                                    Line(DoublePoint(-coord, -coord, -coord), DoublePoint(coord, -coord, -coord)),
                                                    Line(DoublePoint(-coord, -coord, -coord), DoublePoint(-coord, coord, -coord)),
                                                    Line(DoublePoint(-coord, coord, -coord), DoublePoint(coord, coord, -coord)),
                                                    Line(DoublePoint(coord, -coord, -coord), DoublePoint(coord, coord, -coord)),
                                                    Line(DoublePoint(-coord, -coord, coord), DoublePoint(coord, -coord, coord)),
                                                    Line(DoublePoint(-coord, -coord, coord), DoublePoint(-coord, coord, coord)),
                                                    Line(DoublePoint(-coord, coord, coord), DoublePoint(coord, coord, coord)),
                                                    Line(DoublePoint(coord, -coord, coord), DoublePoint(coord, coord, coord)),
                                                    Line(DoublePoint(-coord, -coord, -coord), DoublePoint(-coord, -coord, coord)),
                                                    Line(DoublePoint(-coord, coord, -coord), DoublePoint(-coord, coord, coord)),
                                                    Line(DoublePoint(coord, -coord, -coord), DoublePoint(coord, -coord, coord)),
                                                    Line(DoublePoint(coord, coord, -coord), DoublePoint(coord, coord, coord)),
                                                   });
    this->drawFile(boundcube, 0x000000FF);
}

void VolumeView::wheelEvent(QWheelEvent *event)
{
    float multiplier = 1;
    if (event->delta() > 0)
        multiplier = 1.05;
    else
        multiplier = 0.95;
    _clip_far *= multiplier;
    _current_perspective = transform.Perspective(_clip_near, _clip_far, this->width(), this->height());
    redraw();
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
    float max_size = _objects[0].getScale();
    drawBoundingBox(max_size);
    if (_objectloaded) {
        for(auto it = _objects.begin(); it != _objects.end(); ++it) {
            drawFile((*it).getSegments(), it->_color);
        }
        //drawFile(_object.getSegments(), _object._color);
    }
    std::vector<Line> x_axis;
    x_axis.push_back(Line(DoublePoint(0, 0, 0), DoublePoint(max_size/3, 0, 0)));
    drawFile(x_axis, 0xFF0000FF);

    std::vector<Line> y_axis;
    y_axis.push_back(Line(DoublePoint(0, 0, 0), DoublePoint(0, max_size/3, 0)));
    drawFile(y_axis, 0x00FF00FF);

    std::vector<Line> z_axis;
    z_axis.push_back(Line(DoublePoint(0, 0, 0), DoublePoint(0, 0, max_size/3)));
    drawFile(z_axis, 0x0000FFFF);

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
