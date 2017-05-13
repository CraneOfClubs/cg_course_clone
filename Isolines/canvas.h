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
    QImage * image;
    void loadImage(QImage &image);
    Canvas(QWidget *parent = 0);
protected:
    virtual void resizeEvent(QResizeEvent *e );
    virtual void paintEvent(QPaintEvent *);
private:
    QWidget *_parent;
};

#endif // CANVAS_H
