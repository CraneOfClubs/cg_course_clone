#ifndef FILTERSCLUSTER_H
#define FILTERSCLUSTER_H
#include <QImage>
#include <QProgressBar>

class FiltersCluster : public QObject
{
    Q_OBJECT
signals:
    void alertProgress(int);

public:
    FiltersCluster();
    QImage FloydSteinbergFilter(QImage image, uint8_t red = 2, uint8_t green = 2, uint8_t blue = 2, int32_t matrix_size = 4);
    QImage rotateImage(QImage image, int32_t angle_tt);
    QImage orderedDithering(QImage image, int32_t matrix_size);
    QImage applyFilter(QImage image, int32_t matrix_size);
    QImage grayScale(QImage image);
    QImage Embossing(QImage image);
    QImage Scale(QImage image, int32_t scale_factor);
    QImage Negative(QImage image);
    QImage Edge(QImage image, int32_t treshhold);
    QImage Roberts(QImage image, int32_t threshold);
    QImage Sobel(QImage image, int32_t threshold);
    QImage Watercolor(QImage image, int32_t radius);
    QImage Blur(QImage image, float* matrix, int32_t matrix_size);
    QImage GaussianBlur(QImage image);
    QImage BoxBlur(QImage image);
    QImage SimpleBlur(QImage image);
    QImage GammaCorrection(QImage image, float gamma);
    QImage Sharpness(QImage image);
private:
    uchar getClosestColor(uchar color, int shades);
    std::vector<float> buildMatrix(int size);
};

#endif // FILTERSCLUSTER_H
