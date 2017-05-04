#include "filteredview.h"

void FilteredView::storePixel(int32_t x, int32_t y, uint32_t color){
    if (x < 0 || y < 0 || x >= _borderImage->width() || y >= _borderImage->height())
        return;

    uint32_t offset = x *depth()/8 + y * _borderImage->bytesPerLine();
    _borderImage->bits()[offset] = color >> 24 & 0xFF;
    _borderImage->bits()[offset + 1] = color >> 16 & 0xFF;
    _borderImage->bits()[offset + 2] = color >> 8 & 0xFF;
    _borderImage->bits()[offset + 3] = color & 0xFF;
}

void FilteredView::DrawBorder() {
    memset(_borderImage->bits(), 0, _borderImage->height() * _borderImage->width() * _borderImage->depth() / 8);
    for (int32_t i = 0; i < _borderImage->width(); i++) {
        storePixel(i, 0, 0x000000FF);
        storePixel(i, _borderImage->width() - 3, 0x000000FF);
    }
    for (int32_t i = 0; i < _borderImage->height(); i++) {
        storePixel(0, i, 0x000000FF);
        storePixel(_borderImage->height() - 3, i, 0x000000FF);
    }
    this->repaint();
}

FilteredView::FilteredView(QWidget *parent) : QWidget(parent)
{

}

FilteredView::FilteredView(QWidget *parent, int32_t size_x, int32_t size_y) : QWidget(parent)
{
    _borderImage = new QImage(QSize(size_x + 2, size_y + 2), QImage::Format_RGBA8888);
    memset(_borderImage->bits(), 255, _borderImage->height() * _borderImage->width() * _borderImage->depth() / 8);
    DrawBorder();
    this->setGeometry(750, 50, size_x, size_y);
    _previewImage = new QImage(QSize(size_x + 2, size_y + 2), QImage::Format_RGBA8888);
    setMouseTracking(true);
}

void FilteredView::paintEvent(QPaintEvent * ) {
QPainter painter(this);
    if (!image_loaded) {
        memset(_previewImage->bits(), 255, _previewImage->height() * _previewImage->width() * _previewImage->depth() / 8);
        painter.drawImage(QRect(0, 0, _borderImage->size().width(), _borderImage->size().height()), *_borderImage);
    }

    if (image_loaded) {
        painter.drawImage(QRect(0, 0, _borderImage->size().width(), _borderImage->size().height()), *_borderImage);
        painter.drawImage(QRect(1, 1, _previewImage->size().width(), _previewImage->size().height()), *_previewImage);
    }
}

void FilteredView::loadImage(QImage image) {
    *_previewImage = image;
    image_loaded = true;
    this->repaint();
}
