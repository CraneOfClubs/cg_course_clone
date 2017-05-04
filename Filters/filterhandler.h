#ifndef FILTERHANDLER_H
#define FILTERHANDLER_H
#include <QThread>
#include <QImage>
#include "filterscluster.h"

class FilterHandler: public QThread
{
    Q_OBJECT

signals:
    void valueChanged(int);
    void imageReady(QImage);

public:
    FilterHandler(QObject *parent, int val)
        : QThread(parent), m_currentVal(val)
    {
        filters = new FiltersCluster();
        connect(filters, SIGNAL(alertProgress(int)), this, SLOT(handleProgress(int)));
    }
    void loadImage(QImage image, int32_t f_id, int32_t first, int32_t second, int32_t third);
public slots:
    void handleProgress(int amount);
protected:
    virtual void run();

private:

    QImage apply();
    int m_currentVal;
    int _f_id;
    FiltersCluster* filters;
    QImage _image;
    int32_t _first_param;
    int32_t _second_param;
    int32_t _third_param;
};

#endif // FILTERHANDLER_H
