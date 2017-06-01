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


void DrawingArea::drawLineBresenham(QPoint &from, QPoint &to, uint32_t color) {
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



void DrawingArea::storePixel(int32_t x, int32_t y, uint32_t color){
    if (x < 0 || y < 0 || x >= _image->width() || y >= _image->height())
        return;

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
