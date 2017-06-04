#include "drawingarea.h"

DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent)
{
    _parent = parent;
    _image = new QImage(QSize(this->width(), this->height()), QImage::Format_RGBA8888);
    memset(_image->bits(), 255, _image->height() * _image->width() * _image->depth() / 8);
    setMouseTracking(true);
}

void DrawingArea::loadImage(QImage image)
{
    *_image = image;
    this->repaint();
}

void DrawingArea::clipLine(QPoint &from, QPoint &to) {
    if (from.x() < 0) {
        if (from.x() != to.x()) {
            int newY = from.y() + (to.y() - from.y()) * from.x() / (from.x() - to.x());
            from.setY(newY);
            from.setX(0);
        }
    }
    if (from.x() >= _image->width() - 1) {
        if (to.x() != from.x()) {
            int newY = from.y() + (to.y() - from.y()) * (_image->width() - 1 - from.x()) / (to.x() - from.x());
            from.setY(newY);
            from.setX(_image->width() - 1);
        }
    }
    if (from.y() < 0) {
        if (from.y() != to.y()) {
            int newX = from.x() + (to.x() - from.x()) * from.y() / (from.y() - to.y());
            from.setX(newX);
            from.setY(0);
        }
    }
    if (from.y() >= _image->height() - 1) {
        if (to.y() != from.y()) {
            int newX = from.x() + (to.x() - from.x()) * (_image->height() - 1 - from.y()) / (to.y() - from.y());
            from.setX(newX);
            from.setY(_image->height() - 1);
        }
    }
//    if (from.y() < 0) {
//        from.setY(0);
//    }
//    if (to.y() < 0) {
//        to.setY(0);
//    }
//    if (from.x() >= _image->width())
//        from.setX(_image->width() - 1);
//    if (from.y() >= _image->height())
//        from.setY(_image->height() - 1);
}


bool DrawingArea::drawSimple(QPoint &from, QPoint &to, uint32_t color) {
    if (from.x() == to.x()) {
        if (from.x() < 0 || from.x() >= _image->width())
            return true;
        if (from.y() > to.y())
            std::swap(from, to);
        if (from.y() < 0)
            from.setY(0);
        if (to.y() >= _image->height())
            to.setY(_image->height() - 1);
        if (from.y() >= _image->height())
            return true;
        for (int j = from.y(); j <= to.y(); ++j)
            storePixel(to.x(), j, color);
       return true;
   } else if (from.y() == to.y()) {
        if (from.y() < 0 || from.y() >= _image->height())
            return true;
        if (from.x() > to.x())
            std::swap(from, to);
        if (from.x() < 0)
            from.setX(0);
        if (to.x() >= _image->width())
            to.setX(_image->width() - 1);
        if (from.x() >= _image->width())
            return true;
        for (int i = from.x(); i <= to.x(); ++i) {
            storePixel(i, to.y(), color);
        }
       return true;
   }
       return false;
}


void DrawingArea::drawLineBresenham(QPoint &from, QPoint &to, uint32_t color) {
    if (!drawSimple(from, to, color)) {
        clipLine(from, to);
        if (from.x() == to.x() || from.y() == to.y())
            return;
        clipLine(to, from);
        if (from.x() == to.x() || from.y() == to.y())
            return;
        clipLine(from, to);
        if (from.x() == to.x() || from.y() == to.y())
            return;
        clipLine(to, from);
        if (from.x() == to.x() || from.y() == to.y())
            return;
        if (abs(to.y() - from.y()) >= abs(to.x() - from.x())) {
            if (from.y() > to.y()) {
                std::swap(from, to);
            }
            uint32_t x = from.x();
            int32_t y;
            uint32_t dx = abs(to.x() - from.x());
            uint32_t dy = to.y() - from.y();
            int32_t offset = 2 * dx - dy;
            for (y = from.y(); y <= to.y(); ++y) {
                storePixel( x, y, color);
                if (offset > 0) {
                    x += to.x() > from.x() ? 1 : -1;
                    offset = offset - 2 * dy;
                }
                offset = offset + 2 * dx;
            }
            return;
        }
        if (from.x() > to.x()) {
            std::swap(from, to);
        }
        uint32_t y = from.y();
        int32_t x;
        uint32_t dx = to.x() - from.x();
        uint32_t dy = abs(to.y() - from.y());
        int32_t offset = 2 * dy - dx;
        for (x = from.x(); x <= to.x(); ++x) {
            storePixel( x, y, color);
            if (offset > 0) {
                y += to.y() > from.y() ? 1 : -1;
                offset = offset - 2 * dx;
            }
            offset = offset + 2 * dy;
        }
    }
}



void DrawingArea::storePixel(int32_t x, int32_t y, uint32_t color){
//    if (x < 0 || y < 0 || x >= _image->width() || y >= _image->height())
//        return;
    uint32_t offset = x *_image->depth()/8 + y * _image->bytesPerLine();
    _image->bits()[offset] = color >> 24 & 0xFF;
    _image->bits()[offset + 1] = color >> 16 & 0xFF;
    _image->bits()[offset + 2] = color >> 8 & 0xFF;
    _image->bits()[offset + 3] = color & 0xFF;
}

void DrawingArea::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.drawImage(QRect(0, 0, _image->size().width(), _image->size().height()), *_image);
}

void DrawingArea::fill(uint32_t color) {
    for (int32_t i = 0; i < this->_image->width(); ++i) {
        for (int32_t j = 0; j < this->_image->height(); ++j)
            storePixel(i ,j, color);
    }
}

void DrawingArea::resizeEvent(QResizeEvent *e)
{
    if (!_image_loaded) {
    delete _image;
    _image = new QImage(QSize(this->width(), this->height()), QImage::Format_RGBA8888);
    memset(_image->bits(), 255, _image->height() * _image->width() * _image->depth() / 8);
    repaint();
    }
}
