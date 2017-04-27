#ifndef UTILITY_H
#define UTILITY_H
#include <autopixel.h>
#include <QImage>

namespace MyUtility {

AutoPixel inline getBilinearPixel(QImage &image, float x_n, float y_n) {
    int x = x_n * image.width();
    int y = y_n * image.height();
    if (x < 0 || x >= image.width() || y < 0 || y >= image.height()) {
        return 0;
    }
    float x_c = x_n * image.width() - x;
    float y_c = y_n * image.height()- y;

    int32_t x_s = image.width();
    int32_t y_s = image.height();

    uint8_t channels[4] = {0,0,0,0};
    for (int i = 0; i < 4; i++) {
        channels[i] = image.bits()[y * image.bytesPerLine() + x * 4 + i] * (1.0 - x_c) * (1.0 - y_c)
                + (x >= x_s ? 0 : image.bits()[y * image.bytesPerLine() + (x + 1) * 4 + i] * x_c * (1.0 - y_c))
                + (y >= y_s ? 0 : image.bits()[(y + 1) * image.bytesPerLine() + x * 4 + i] * y_c * (1.0 - x_c))
                + (y >= y_s || x >= x_s ? 0 : image.bits()[(y + 1) * image.bytesPerLine() + (x + 1) * 4 + i] * x_c * y_c);
    }
    AutoPixel pixel(channels[0], channels[1], channels[2], channels[3]);
    return pixel;
}

AutoPixel inline getBilinearPixelWithOffset(QImage &image, float x_n, float y_n, int x_offset, int y_offset) {
    int x = x_n * image.width() + x_offset;
    int y = y_n * image.height() + y_offset;
    if (x < 0 || x >= image.width() || y < 0 || y >= image.height()) {
        return 0;
    }
    float x_c = x_n * image.width() - x + x_offset;
    float y_c = y_n * image.height()- y + y_offset;

    int32_t x_s = image.width();
    int32_t y_s = image.height();

    uint8_t channels[4] = {0,0,0,0};
    for (int i = 0; i < 4; i++) {
        channels[i] = image.bits()[y * image.bytesPerLine() + x * 4 + i] * (1.0 - x_c) * (1.0 - y_c)
                + (x >= x_s ? 0 : image.bits()[y * image.bytesPerLine() + (x + 1) * 4 + i] * x_c * (1.0 - y_c))
                + (y >= y_s ? 0 : image.bits()[(y + 1) * image.bytesPerLine() + x * 4 + i] * y_c * (1.0 - x_c))
                + (y >= y_s || x >= x_s ? 0 : image.bits()[(y + 1) * image.bytesPerLine() + (x + 1) * 4 + i] * x_c * y_c);
    }
    AutoPixel pixel(channels[0], channels[1], channels[2], channels[3]);
    return pixel;
}

QImage inline drawBorders(QImage &image) {
    for (int i = 0; i < image.width(); ++i) {
        image.bits()[i*4] = 0;
        image.bits()[i*4 + 1] = 0;
        image.bits()[i*4 + 2] = 0;
        image.bits()[i*4 + 3] = 255;
    }

    for (int i = (image.height()) * image.width(); i < (image.width()) * image.height(); ++i) {
        image.bits()[i*4] = 0;
        image.bits()[i*4 + 1] = 0;
        image.bits()[i*4 + 2] = 0;
        image.bits()[i*4 + 3] = 255;
    }
}

QImage inline scaleImage(QImage &image, float factor, int width_max = 0, int height_max = 0) {
    if (factor == 1) {
        return QImage(image);
    }
    int width;
    int height;
    if (factor > 1) {
        width = image.width() * factor;
        height = image.height() * factor;
        if (width > width_max) {
            width = width_max;
        }
        if (height > height_max) {
            height = height_max;
        }
    } else {
        width = image.width() * factor;
        height = image.height() * factor;
    }
    QImage scaled(width, height, QImage::Format_RGBA8888);

    int x_offset = factor > 1 && image.width() * factor >= width_max ? (image.width() - image.width() / factor) / 2 : 0;
    int y_offset = factor > 1 && image.height() * factor >= height_max ? (image.height() - image.height() / factor) / 2 : 0;
    for (int j = 0; j < scaled.height(); ++j) {
        for (int i = 0; i < scaled.width(); ++i) {
            float x_norm = (i + x_offset * factor) / (image.width() * factor);
            float y_norm = (j + y_offset * factor) / (image.height() * factor);
            auto biba = getBilinearPixel(image, x_norm, y_norm);
            uint32_t offset = i * scaled.depth()/8 + j * scaled.bytesPerLine();
            scaled.bits()[offset] = biba.getRed();
            scaled.bits()[offset + 1] = biba.getGreen();
            scaled.bits()[offset + 2] = biba.getBlue();
            scaled.bits()[offset + 3] = biba.getAlpha();
        }
    }
    return scaled;
}

QImage inline scaleImage(QImage &image, int width, int height) {
    if (image.width() < width && image.height() < height) {
        return image;
    }
    float x_factor = (float) width / (float) image.width();
    float y_factor = (float) height / (float) image.height();
    return scaleImage(image, std::min(x_factor, y_factor));
}

}




#endif // UTILITY_H
