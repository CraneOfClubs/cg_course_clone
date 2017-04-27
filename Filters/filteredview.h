#ifndef FILTEREDVIEW_H
#define FILTEREDVIEW_H

#include <QWidget>
#include <QImage>
#include <QPainter>


class FilteredView : public QWidget
{
    Q_OBJECT
private:
    QImage * _previewImage;
    bool image_loaded = false;
public:
    explicit FilteredView(QWidget *parent = 0);
    explicit FilteredView(QWidget *parent, int32_t size_x, int32_t size_y);
    void loadImage(QImage image);
signals:

public slots:
protected:
    virtual void paintEvent(QPaintEvent *);
};
#endif // FILTEREDVIEW_H
