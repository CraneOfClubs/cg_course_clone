#include "canvas.h"

void Canvas::loadImage(QImage &image)
{
    this->image = &image;
}

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    _parent = parent;
    image = new QImage(QSize(this->width(), this->height()), QImage::Format_RGBA8888);
    memset(image->bits(), 255, image->height() * image->width() * image->depth() / 8);
}

void Canvas::resizeEvent(QResizeEvent *e)
{
    MainWindow *mw = dynamic_cast<MainWindow*>(_parent);
    if (mw->_inited) {
    QImage buf = mw->getEngine()->generateImage(this->width(), this->height());
    delete image;
    *image = buf;
    }
}

void Canvas::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.drawImage(QRect(0, 0, image->size().width(), image->size().height()), *image);
}
