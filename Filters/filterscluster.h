#ifndef FILTERSCLUSTER_H
#define FILTERSCLUSTER_H
#include <QImage>
#include <QProgressBar>

class FiltersCluster
{
public:
    FiltersCluster();
    QImage FloydSteinbergFilter(QImage image, QProgressBar *tracking, uint8_t red = 2, uint8_t green = 2, uint8_t blue = 2, int32_t matrix_size = 4);
    QImage rotateImage(QImage image, QProgressBar *tracking, int32_t angle_tt);
    QImage orderedDithering(QImage image, QProgressBar *tracking, int32_t matrix_size);
    QImage applyFilter(QImage image, int32_t matrix_size);
    QImage grayScale(QImage image, QProgressBar *tracking);
    QImage Embossing(QImage image, QProgressBar *tracking);
    QImage Scale(QImage image, QProgressBar *tracking, int32_t scale_factor);
    QImage Negative(QImage image, QProgressBar *tracking);
    QImage Edge(QImage image, QProgressBar *tracking, int32_t treshhold);
    QImage Roberts(QImage image, QProgressBar *tracking, int32_t threshold);
    QImage Sobel(QImage image, QProgressBar *tracking, int32_t threshold);
    QImage Watercolor(QImage image, QProgressBar *tracking, int32_t radius);
    QImage Blur(QImage image, QProgressBar *tracking, float* matrix, int32_t matrix_size);
    QImage GaussianBlur(QImage image, QProgressBar *tracking);
    QImage BoxBlur(QImage image, QProgressBar *tracking);
    QImage SimpleBlur(QImage image, QProgressBar *tracking);
    QImage GammaCorrection(QImage image, QProgressBar *tracking, float gamma);
    QImage Sharpness(QImage image, QProgressBar *tracking);
private:
    uchar getClosestColor(uchar color, int shades);
    std::vector<float> buildMatrix(int size);
};

#endif // FILTERSCLUSTER_H
