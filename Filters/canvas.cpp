#include "canvas.h"

Canvas::Canvas(QWidget *parent, int32_t size_x, int32_t size_y, MainWindow *parentwindow) : QWidget(parent)
{
    _parentwindow = parentwindow;
    _parent = parent;
    this->setGeometry(50, 50, size_x, size_y);
    _image = new QImage(QSize(size_x, size_y), QImage::Format_RGBA8888);
    scaledImage = new QImage(QSize(size_x + 2, size_y + 2), QImage::Format_RGBA8888);
    origScaledImage = new QImage(QSize(size_x + 2, size_y + 2), QImage::Format_RGBA8888);

    _borderImage = new QImage(QSize(size_x + 2, size_y + 2), QImage::Format_RGBA8888);
    memset(_borderImage->bits(), 255, _borderImage->height() * _borderImage->width() * _borderImage->depth() / 8);
    DrawBorder();

    setMouseTracking(true);
}

void Canvas::drawSelectRect(int32_t x, int32_t y) {
    *scaledImage = *origScaledImage;
    double factor = (double)scaledImage->height() / (double)_image->height();
    int32_t size_of_side = round(factor * 300);
    for (int32_t i = -1 * (size_of_side / 2) + x; i < (size_of_side / 2) + x; i++) {
        storePixel(i, -1 * (size_of_side / 2) + y, 0x000000FF);
        storePixel(i, (size_of_side / 2) + y, 0x000000FF);
    }
    for (int32_t i = -1 * (size_of_side / 2) + y; i < (size_of_side / 2) + y; i++) {
        storePixel( -1 * (size_of_side / 2) + x, i, 0x000000FF);
        storePixel((size_of_side / 2) + x, i, 0x000000FF);
    }
    this->repaint();
}


void Canvas::mouseMoveEvent(QMouseEvent* event)
{
    if (lmb_pressed) {
        int factor = round(((float)scaledImage->height() / (float)_image->height()) * 150);
        if (scaledImage->height() < 300 && scaledImage->width() < 300) {
            return;
        }

        if (event->x() - factor < 1 ||  event->x() + factor > scaledImage->width() || event->y() - factor < 1 ||  event->y() + factor > scaledImage->height() )
            return;
        else {
            drawSelectRect(event->x(), event->y());
            double non_m_factor = (double)scaledImage->height() / (double)_image->height();
            int32_t x_center =round((double)(event->x()) / non_m_factor);
            int32_t y_center =round((double)(event->y()) / non_m_factor);
            setCenterOfNonScaled(x_center, y_center);
            QImage buf_image(QSize(300, 300), QImage::Format_RGBA8888);
            int32_t x_start = x_center - 150;
            int32_t y_start = y_center - 150;

            memset(buf_image.bits(), 0 , buf_image.width() * buf_image.height() * 4);

            for (int i = 0; i < 300; ++i) {
                memcpy(buf_image.bits() + i * buf_image.bytesPerLine(),
                       _image->bits() + (y_start + i) * _image->bytesPerLine() + x_start * _image->depth() / 8,
                       300 * _image->depth() / 8);
            }
            _parentwindow->selectHandler(buf_image);
        }

    }

}

void Canvas::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        lmb_pressed = false;
        if (scaledImage->height() < 300 && scaledImage->width() < 300) {
            if (event->x() < scaledImage->height() && event->y() < scaledImage->width()) {
                _parentwindow->selectHandler(*scaledImage);
            }
        }
        float factor_f = ((float)scaledImage->height() / (float)_image->height()) * 150;
        int factor = round(factor_f);
        if (event->x() - factor < 1 ||  event->x() + factor > scaledImage->width() || event->y() - factor < 1 ||  event->y() + factor > scaledImage->height() )
            return;
        else {
            drawSelectRect(event->x(), event->y());
            double non_m_factor = (double)scaledImage->height() / (double)_image->height();
            int32_t x_center =round((double)(event->x()) / non_m_factor);
            int32_t y_center =round((double)(event->y()) / non_m_factor);
            setCenterOfNonScaled(x_center, y_center);
            QImage buf_image(QSize(300, 300), QImage::Format_RGBA8888);
            int32_t x_start = x_center - 150;
            int32_t y_start = y_center - 150;

            memset(buf_image.bits(), 0 , buf_image.width() * buf_image.height() * 4);

            for (int i = 0; i < 300; ++i) {
                memcpy(buf_image.bits() + i * buf_image.bytesPerLine(),
                       _image->bits() + (y_start + i) * _image->bytesPerLine() + x_start * _image->depth() / 8,
                       300 * _image->depth() / 8);
            }
            _parentwindow->selectHandler(buf_image);
        }
    }
}

void Canvas::storePixel(int32_t x, int32_t y, uint32_t color){
    if (x < 0 || y < 0 || x >= scaledImage->width() || y >= scaledImage->height())
        return;

    uint32_t offset = x *depth()/8 + y * scaledImage->bytesPerLine();
    scaledImage->bits()[offset] = color >> 24 & 0xFF;
    scaledImage->bits()[offset + 1] = color >> 16 & 0xFF;
    scaledImage->bits()[offset + 2] = color >> 8 & 0xFF;
    scaledImage->bits()[offset + 3] = color & 0xFF;
}

void Canvas::storeBorderPixel(int32_t x, int32_t y, uint32_t color){
    if (x < 0 || y < 0 || x >= _borderImage->width() || y >= _borderImage->height())
        return;

    uint32_t offset = x *depth()/8 + y * _borderImage->bytesPerLine();
    _borderImage->bits()[offset] = color >> 24 & 0xFF;
    _borderImage->bits()[offset + 1] = color >> 16 & 0xFF;
    _borderImage->bits()[offset + 2] = color >> 8 & 0xFF;
    _borderImage->bits()[offset + 3] = color & 0xFF;
}

void Canvas::DrawBorder() {
    memset(_borderImage->bits(), 0, _borderImage->height() * _borderImage->width() * _borderImage->depth() / 8);
    for (int32_t i = 0; i < _borderImage->width(); i++) {
        storeBorderPixel(i, 0, 0x000000FF);
        storeBorderPixel(i, _borderImage->width() - 3, 0x000000FF);
    }
    for (int32_t i = 0; i < _borderImage->height(); i++) {
        storeBorderPixel(0, i, 0x000000FF);
        storeBorderPixel(_borderImage->height() - 3, i, 0x000000FF);
    }
    this->repaint();
}

void Canvas::paintEvent(QPaintEvent * ) {
QPainter painter(this);
    if (!image_loaded) {
        memset(scaledImage->bits(), 255, scaledImage->height() * scaledImage->width() * scaledImage->depth() / 8);
        painter.drawImage(QRect(0, 0, _borderImage->size().width(), _borderImage->size().height()), *_borderImage);
    }

    if (image_loaded) {
        painter.drawImage(QRect(0, 0, _borderImage->size().width(), _borderImage->size().height()), *_borderImage);
        painter.drawImage(QRect(1, 1, scaledImage->size().width() - 2, scaledImage->size().height() - 2), *scaledImage);
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lmb_pressed = true;
    }
}

void Canvas::setCenterOfNonScaled(int32_t x, int32_t y) {
    c_of_nsc_x = x;
    c_of_nsc_y = y;
}

void Canvas::loadImage(QImage image) {
    *_image = image;
    *scaledImage = MyUtility::scaleImage(*_image, 300, 300);
    *origScaledImage = *scaledImage;
    image_loaded = true;
    this->repaint();
}
