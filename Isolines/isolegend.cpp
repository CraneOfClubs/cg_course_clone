#include "isolegend.h"
#include <QWidget>

IsoLegend::IsoLegend(QWidget *parent) : QWidget(parent)
{
    _parent = parent;
    this->setMinimumHeight(50);
    this->setMinimumWidth(300);
    this->setMaximumHeight(50);
    this->setMaximumWidth(2000);
    _image = new QImage(QSize(this->width(), this->height()), QImage::Format_RGBA8888);
    _visible_image = new QImage(QSize(this->width(), this->height()), QImage::Format_RGBA8888);
    memset(_image->bits(), 255, _image->height() * _image->width() * _image->depth() / 8);
}

void IsoLegend::resizeEvent(QResizeEvent *e)
{
    *_image = QImage(QSize(this->width(), this->height()), QImage::Format_RGBA8888);
    recalcImage();
}

void IsoLegend::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.drawImage(QRect(0, 0, _visible_image->size().width(), _visible_image->size().height()), *_visible_image);
}

void IsoLegend::storePixel(QImage *image, int32_t x, int32_t y, uint32_t color){
    if (x < 0 || y < 0 || x >= image->width() || y >= image->height())
        return;

    uint32_t offset = x *image->depth()/8 + y * image->bytesPerLine();
    image->bits()[offset] = color >> 24 & 0xFF;
    image->bits()[offset + 1] = color >> 16 & 0xFF;
    image->bits()[offset + 2] = color >> 8 & 0xFF;
    image->bits()[offset + 3] = color & 0xFF;
}

void IsoLegend::setCurrentValue(double value) {
    *_visible_image = *_image;
    MainWindow *mw = dynamic_cast<MainWindow*>(_parent);
    auto limits = mw->getEngine()->getZLimits();
    double diff = (limits.second - limits.first);
    double len = (value - limits.first) * this->width();
    int32_t i_value = len / diff;
    for (int32_t i = 1; i < this->height() / 2; ++i) {
        storePixel(_visible_image, i_value, i, 0x000000FF);
        storePixel(_visible_image, i_value + 1, i, 0x000000FF);
        storePixel(_visible_image, i_value - 1, i, 0x000000FF);
    }

    QPainter p(_visible_image);
    p.setPen(QPen(Qt::black));
    p.setFont(QFont("Times", 7, QFont::Bold));
    if (i_value + 25 >= this->width())
        p.drawText(QPoint(i_value - 25, 45), QString::number(value, 'f', 2));
    else if (i_value - 13 < 0)
        p.drawText(QPoint(2, 45), QString::number(value, 'f', 2));
    else
        p.drawText(QPoint(i_value - 13, 45), QString::number(value, 'f', 2));
    this->repaint();
}

void IsoLegend::recalcImage() {
    memset(_image->bits(), 255, _image->height() * _image->width() * _image->depth() / 8);
    for (int32_t i = 0; i < this->width(); ++i) {
        storePixel(_image, i, 0, 0x000000FF);
        storePixel(_image, i, this->height() - 1, 0x000000FF);
        storePixel(_image, i, this->height() / 2, 0x000000FF);
    }
    MainWindow *mw = dynamic_cast<MainWindow*>(_parent);
    int32_t levels_amount = mw->getEngine()->getAmountOfLevels();
    int32_t offset = this->width() / levels_amount;
    int32_t cur_pos = 0;
    int32_t cur_start = 0;
    double cur_off_f = mw->getEngine()->getZLimits().first;
    mw->getEngine()->findZLimits(mw->canvas->width(), mw->canvas->height());
    double offset_f = (mw->getEngine()->getZLimits().second - mw->getEngine()->getZLimits().first) / levels_amount;
    QPainter p(_image);
    p.setPen(QPen(Qt::black));
    p.setFont(QFont("Times", 7, QFont::Bold));
    auto colors = mw->getEngine()->getAllColors();
    for (int32_t j = 0; j < levels_amount; j++) {
        cur_pos += offset;
        for (int32_t k = cur_start + 1; k < cur_pos; k++) {
            for (int32_t i = 1; i < this->height() / 2; ++i) {
                storePixel(_image, k, i, colors[j]);
            }
        }
        for (int32_t i = 0; i < this->height() / 2; ++i) {
            storePixel(_image, cur_pos, i, 0x000000FF);
        }
        if (j != 0)
            p.drawText(QPoint(cur_start - 15, 34), QString::number(cur_off_f, 'f', 2));
        cur_start += offset;
        cur_off_f += offset_f;
    }
    p.drawText(QPoint(1, 34), QString::number(mw->getEngine()->getZLimits().first, 'f', 2));
    p.drawText(QPoint(cur_start - 25, 34), QString::number(cur_off_f, 'f', 2));
    for (int32_t i = 0; i < this->height(); ++i) {
        storePixel(_image, 0, i, 0x000000FF);
        storePixel(_image, this->width() - 1, i, 0x000000FF);
    }
    *_visible_image = *_image;
    this->repaint();
}
