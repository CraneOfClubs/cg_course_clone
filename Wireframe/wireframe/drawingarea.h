#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>

class DrawingArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = 0);
    QImage * _image;
    void loadImage(QImage image);
    QWidget *_parent;
    void storePixel(int32_t x, int32_t y, uint32_t color);
    void fill(uint32_t color);
    void drawLineBresenham(QPoint &from, QPoint &to, uint32_t color);
private:
    void clipLine(QPoint &from, QPoint &to);
    bool drawSimple(QPoint &from, QPoint &to, uint32_t color);
protected:
    bool _image_loaded = false;
    virtual void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *e);
signals:


public slots:
};

#endif // DRAWINGAREA_H
