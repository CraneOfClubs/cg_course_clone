#ifndef ISOLEGEND_H
#define ISOLEGEND_H
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <mainwindow.h>

class MainWindow;

class IsoLegend : public QWidget
{
private:
    QWidget *_parent;
    QImage *_image;
    QImage *_visible_image;
    void storePixel(QImage *image, int32_t x, int32_t y, uint32_t color);
public:
    IsoLegend(QWidget *parent = 0);
    void recalcImage();
    void setCurrentValue(double value);
protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *);
};

#endif // ISOLEGEND_H
