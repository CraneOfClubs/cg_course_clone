#ifndef PICVIEW_H
#define PICVIEW_H

#include <QWidget>
#include <QImage>
#include <QPainter>


class PicView : public QWidget
{
    Q_OBJECT
private:


public:
    bool image_loaded = false;
    QImage * _previewImage;
    explicit PicView(QWidget *parent = 0);   
    explicit PicView(QWidget *parent, int32_t size_x, int32_t size_y);
    void loadImage(QImage image);
signals:

public slots:
protected:
    virtual void paintEvent(QPaintEvent *);
};

#endif // PICVIEW_H
