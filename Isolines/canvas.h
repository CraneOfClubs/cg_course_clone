#ifndef CANVAS_H
#define CANVAS_H
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <mainwindow.h>

class MainWindow;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    QImage * _image;
    void loadImage(QImage image);
    Canvas(QWidget *parent = 0);
    void drawMesh(int32_t rows, int32_t cols);
    void storePixel(QImage *image, int32_t x, int32_t y, uint32_t color);
    void drawIsolines(int32_t rows, int32_t cols);
    void redraw();
protected:
    virtual void resizeEvent(QResizeEvent *e );
    virtual void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
//    int32_t _rows;
//    int32_t _cols;
    QWidget *_parent;
    void drawLineBresenham(QPoint &from, QPoint &to, uint32_t color);
    void calcBrOffset(int32_t offset);
};

#endif // CANVAS_H
