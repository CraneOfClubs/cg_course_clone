#ifndef FILTEREDVIEW_H
#define FILTEREDVIEW_H

#include <QWidget>
#include <QImage>
#include <QPainter>


class FilteredView : public QWidget
{
    Q_OBJECT
private:

    bool image_loaded = false;
    void storePixel(int32_t x, int32_t y, uint32_t color);
    void DrawBorder();
public:
    explicit FilteredView(QWidget *parent = 0);
    explicit FilteredView(QWidget *parent, int32_t size_x, int32_t size_y);
    void loadImage(QImage image);
    QImage * _borderImage;
    QImage * _previewImage;
signals:

public slots:
protected:
    virtual void paintEvent(QPaintEvent *);
};
#endif // FILTEREDVIEW_H
