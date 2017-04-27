#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QFile>
#include <QPainter>
#include <utility.h>
#include <QtGui/QMouseEvent>
#include <mainwindow.h>

class MainWindow;

class Canvas : public QWidget
{
    Q_OBJECT

private:
    QImage* fullImage;
    bool image_loaded = false;
    int32_t c_of_nsc_x = -1;
    int32_t c_of_nsc_y = -1;
    MainWindow *_parentwindow;
    QWidget *_parent;
protected:
    virtual void paintEvent(QPaintEvent *);
    //virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
//    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

public:
    void drawSelectRect(int32_t x, int32_t y);
    Canvas(QWidget *parent = 0, int32_t size_x = 50, int32_t size_y = 50, MainWindow *parentwindow = 0);
    QImage* _image;
    QImage* origScaledImage;
    QImage* scaledImage;
    void loadImage(QImage image);
    void setPixel(int32_t x, int32_t y, uint32_t color);
    void storePixel(int32_t x, int32_t y, uint32_t color);
    void setCenterOfNonScaled(int32_t x, int32_t y);
signals:



public slots:
};

#endif // CANVAS_H
