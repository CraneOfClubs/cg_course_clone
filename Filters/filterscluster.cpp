#include "filterscluster.h"
#include "utility.h"

FiltersCluster::FiltersCluster()
{

}

uchar fixC(int32_t color) {
    if (color > 255) {
        return 255;
    }
    if (color < 0) {
        return 0;
    }
    return color;
}

uchar FiltersCluster::getClosestColor(uchar color, int shades) {
    float period = 255.0 / (float) (shades - 1);
    return period * (int) ((color + period / 2) / period);
}

QImage FiltersCluster::FloydSteinbergFilter(QImage image, uint8_t red, uint8_t green, uint8_t blue, int32_t matrix_size) {
    uint8_t color_settings[3];
    color_settings[0] = red;
    color_settings[1] = green;
    color_settings[2] = blue;
    for (int32_t j = 0; j < image.height() - 1; j++) {
        for (int32_t i = 1; i < image.width() - 1; i++) {
            for (int32_t k = 0; k < 3; k++) {
                uint8_t old_p = image.bits()[j * image.width() * 4 + i * 4 + k];
                uint8_t new_p = getClosestColor(old_p, color_settings[k]);
                image.bits()[j * image.width() * 4 + i * 4 + k] = new_p;
                int error = old_p - new_p;
                int ul = image.bits()[ j * image.width() * 4 + (i + 1) * 4 + k] + error * 7.0/16.0;
                int uu = image.bits()[(j + 1) * image.width() * 4 + (i - 1) * 4 + k] + error * 3.0/16.0;
                int lu = image.bits()[(j + 1) * image.width() * 4 + i * 4 + k] + error * 5.0/16.0;
                int ll = image.bits()[(j + 1) * image.width() * 4 + (i + 1) * 4 + k] + error * 1.0/16.0;
                image.bits()[ j * image.width() * 4 + (i + 1) * 4 + k] = fixC(ul);
                image.bits()[(j + 1) * image.width() * 4 + (i - 1) * 4 + k] = fixC(uu);
                image.bits()[(j + 1) * image.width() * 4 + (i) * 4 + k] = fixC(lu);
                image.bits()[(j + 1) * image.width() * 4 + (i + 1) * 4 + k] = fixC(ll);
            }
        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    emit alertProgress(100);
    return image;
}

float PI = 3.1415926535897f;

QImage FiltersCluster::rotateImage(QImage image, int32_t angle_tt) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    float angle = (float) angle_tt / 180.0 * PI;
    for (int32_t j = 0; j < image.height(); j++) {
        for (int32_t i = 0; i < image.width(); i++) {
            int x = (i - image.width() / 2) * std::cos(angle) - (j - image.height() / 2) * std::sin(angle);
            int y = (i - image.width() / 2) * std::sin(angle) + (j - image.height() / 2) * std::cos(angle);
            auto Pixel = MyUtility::getBilinearPixelWithOffset(image, (float)x / image.width(), (float)y / image.height(), image.width() / 2, image.height() / 2);
            filtered_image.bits()[j * filtered_image.width() * 4 + i * 4] = Pixel.getRed();
            filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + 1] = Pixel.getGreen();
            filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + 2] = Pixel.getBlue();
            filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + 3] = Pixel.getAlpha();

        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    return filtered_image;
}


QImage FiltersCluster::orderedDithering(QImage image, int32_t matrix_size) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    int done_size = 2;
    std::vector<int> matrix({0, 2, 3, 1});
    while (done_size < matrix_size) {
        std::vector<int> doubled(done_size * 2 * done_size * 2);
        for (int32_t j = 0; j < done_size; j++) {
            for (int32_t i = 0; i < done_size; i++) {
                doubled[j * done_size * 2 + i] = 4 * matrix[j * done_size + i];
            }
        }
        for (int32_t j = 0; j < done_size; j++) {
            for (int32_t i = 0; i < done_size; i++) {
                doubled[j * done_size * 2 + done_size + i] = 4 * matrix[j * done_size + i] + 2;
            }
        }
        for (int32_t j = 0; j < done_size; j++) {
            for (int32_t i = 0; i < done_size; i++) {
                doubled[(j + done_size) * done_size * 2 + i] = 4 * matrix[j * done_size + i] + 3;
            }
        }
        for (int32_t j = 0; j < done_size; j++) {
            for (int32_t i = 0; i < done_size; i++) {
                doubled[(j + done_size) * done_size * 2 + done_size + i] = 4 * matrix[j * done_size + i] + 1;
            }
        }
        matrix = doubled;
        done_size = done_size * 2;
    }
    std::vector<float> float_matrix(matrix_size * matrix_size);
    for (int32_t i = 0; i < matrix_size * matrix_size; i++) {
        float_matrix[i] = (float) matrix[i] / (float) (matrix_size * matrix_size);
    }

    for (int32_t j = 0; j < filtered_image.height(); j++) {
        for (int32_t i = 0; i < filtered_image.width(); i++) {
            int matrix_value = float_matrix[(j % matrix_size) * matrix_size + (i % matrix_size)] * 255;
            for (int32_t k = 0; k < 4; ++k) {
                uchar pixel_value = image.bits()[j * image.width() * 4 + i * 4 + k];
                if (pixel_value > matrix_value)
                    filtered_image.bits()[j * image.width() * 4 + i * 4 + k] = 255;
                else
                    filtered_image.bits()[j * image.width() * 4 + i * 4 + k] = 0;
            }
        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    emit alertProgress(100);
    return filtered_image;
}



QImage FiltersCluster::grayScale(QImage image) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    for (int32_t j = 0; j < filtered_image.height(); j++) {
        for (int32_t i = 0; i < filtered_image.width(); i++) {
            int cur_char = j * image.bytesPerLine() + i * image.depth() / 8;
            filtered_image.bits()[cur_char] = 0.2126 * (double)image.bits()[cur_char] + 0.7152 * (double)image.bits()[cur_char + 1] + 0.0722 * (double)image.bits()[cur_char + 2];
            filtered_image.bits()[cur_char + 1] = 0.2126 * (double)image.bits()[cur_char] + 0.7152 * (double)image.bits()[cur_char + 1] + 0.0722 * (double)image.bits()[cur_char + 2];
            filtered_image.bits()[cur_char + 2] = 0.2126 * (double)image.bits()[cur_char] + 0.7152 * (double)image.bits()[cur_char + 1] + 0.0722 * (double)image.bits()[cur_char + 2];
            filtered_image.bits()[cur_char + 3] = image.bits()[cur_char + 3];
        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    return filtered_image;
}

int inline invert_b(int max, int c) {
    if (c < 0) {
        return - c - 1;
    }
    if (c >= max) {
        return 2 * max - c - 1;
    }
    return c;
}

int32_t bound (int32_t val) {
    if (val < 0) {
        val = 0;
    }
    if (val > 255) {
        val = 255;
    }
    return val;
}

QImage FiltersCluster::Embossing(QImage image) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    float matrix[9] ={ 0.0, 1.0, 0.0, -1.0, 0.0, 1.0, 0.0, -1.0, 0.0};
    int matrixSize = 3;
    for (int32_t j = 0; j < filtered_image.height(); j++) {
        for (int32_t i = 0; i < filtered_image.width(); i++) {
            float sum[4] = {0.0, 0.0, 0.0, 0.0};
            for (int32_t i1 = 0; i1 < matrixSize; i1++) {
                for (int32_t j1 = 0; j1 < matrixSize; j1++) {
                    int x = invert_b(image.width(), i + j1 - matrixSize / 2);
                    int y = invert_b(image.height(), j + i1 - matrixSize / 2);
                    for (int32_t k = 0; k < 3; ++k) {
                        sum[k] += matrix[i1 * matrixSize + j1] * image.bits()[ y * image.width() * 4 + x * 4 + k];
                    }
                }
            }
            for (int32_t i1 = 0; i1 < 3; i++) {
                filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + i1] = bound((int)sum[i1] + 128);
            }
            filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + 3] = image.bits()[j * image.width() * 4 + i * 4 + 3];
        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    return filtered_image;
}

QImage FiltersCluster::Scale(QImage image, int32_t scale_factor) {
    float scale_val;
    if (scale_factor == 0) {
        return image;
    } else if (scale_factor < 0) {
        scale_val = 1.0 / ((float)scale_factor * -1);
    } else {
        scale_val = scale_factor;
    }
    emit alertProgress(100);
    return MyUtility::scaleImage(image, scale_val, 300, 300);
}

QImage FiltersCluster::Negative(QImage image) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    for (int32_t j = 0; j < filtered_image.height(); j++) {
        for (int32_t i = 0; i < filtered_image.width(); i++) {
            int index = j * image.bytesPerLine() + i * 4;
            filtered_image.bits()[index] = 255 - image.bits()[index];
            filtered_image.bits()[index + 1] = 255 - image.bits()[index + 1];
            filtered_image.bits()[index + 2] = 255 - image.bits()[index + 2];
            filtered_image.bits()[index + 3] = image.bits()[index + 3];
        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    return filtered_image;
}

QImage FiltersCluster::Edge(QImage image, int32_t threshold)
{
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    image = grayScale(image);
    float matrix[9] ={0.0, -1.0,  0.0, -1.0,  4.0, -1.0, 0.0, -1.0, 0.0};
    int32_t matrixSize = 3;
    for (int32_t j = 0; j < filtered_image.height(); j++) {
        for (int32_t i = 0; i < filtered_image.width(); i++) {
            int32_t sum = 0;
            for (int32_t j1 = 0; j1 < matrixSize; j1++) {
                for (int32_t i1 = 0; i1 < matrixSize; i1++) {
                    int32_t x = invert_b(image.width(), i + i1 - matrixSize / 2);
                    int32_t y = invert_b(image.height(), j + j1 - matrixSize / 2);
                    sum += matrix[j1 * matrixSize + i1] * image.bits()[ y * image.width() * 4 + x * 4];
                }
            }
            int32_t res = sum > threshold ? 255 : 0;
            for (int32_t i1 = 0; i1 < 3; i1++) {
                filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + i1] = res;
            }
            filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + 3] = image.bits()[j * image.width() * 4 + i * 4 + 3];
        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    return filtered_image;
}

QImage FiltersCluster::Roberts(QImage image, int32_t threshold) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    image = grayScale(image);
    int32_t xmatrix[4] ={1, 0, 0, -1};
    int32_t ymatrix[4] ={0, 1, -1, 0};
    int32_t matrixSize = 2;
    int32_t _x = 0;
    int32_t _y = 0;
    for (int32_t j = 0; j < filtered_image.height(); j++) {
        for (int32_t i = 0; i < filtered_image.width(); i++) {
            _x = 0;
            _y = 0;
            for (int32_t j1 = 0; j1 < matrixSize; j1++) {
                for (int32_t i1 = 0; i1 < matrixSize; i1++) {
                    int x = invert_b(image.width(), i + i1);
                    int y = invert_b(image.height(), j + j1);
                    _x += xmatrix[j1 * matrixSize + i1] * image.bits()[ y * image.width() * 4 + x * 4];
                    _y += ymatrix[j1 * matrixSize + i1] * image.bits()[ y * image.width() * 4 + x * 4];
                }
            }
            float back = std::sqrt(_x * _x + _y * _y);
            int32_t res = back > threshold ? 255 : 0;
            for (int32_t i1 = 0; i1 < 3; i1++) {
                filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + i1] = res;
            }
            filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + 3] = image.bits()[j * image.width() * 4 + i * 4 + 3];
        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    return filtered_image;
}

QImage FiltersCluster::Sobel(QImage image, int32_t threshold) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    image = grayScale(image);
    int32_t xmatrix[9] ={-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int32_t ymatrix[9] ={-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int32_t matrixSize = 3;
    int32_t _x = 0;
    int32_t _y = 0;
    for (int32_t j = 0; j < filtered_image.height(); j++) {
        for (int32_t i = 0; i < filtered_image.width(); i++) {
            _x = 0;
            _y = 0;
            for (int32_t j1 = 0; j1 < matrixSize; j1++) {
                for (int32_t i1 = 0; i1 < matrixSize; i1++) {
                    int32_t x = invert_b(image.width(), i + i1 - matrixSize / 2);
                    int32_t y = invert_b(image.height(), j + j1 - matrixSize / 2);
                    _x += xmatrix[j1 * matrixSize + i1] * image.bits()[ y * image.width() * 4 + x * 4];
                    _y += ymatrix[j1 * matrixSize + i1] * image.bits()[ y * image.width() * 4 + x * 4];
                }
            }
            float back = std::sqrt(_x * _x + _y * _y);
            int32_t res = back > threshold ? 255 : 0;
            for (int32_t i1 = 0; i1 < 3; i1++) {
                filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + i1] = res;
            }
            filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + 3] = image.bits()[j * image.width() * 4 + i * 4 + 3];
        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    return filtered_image;
}


QImage FiltersCluster::Blur(QImage image, float* matrix, int32_t matrix_size) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    for (int32_t j = 0; j < filtered_image.height(); j++) {
        for (int32_t i = 0; i < filtered_image.width(); i++) {
            float sum[4] = {0.0, 0.0, 0.0, 0.0};
            for (int32_t j1 = 0; j1 < matrix_size; j1++) {
                for (int32_t i1 = 0; i1 < matrix_size; i1++) {
                    int x = invert_b(image.width(), i + i1 - matrix_size / 2);
                    int y = invert_b(image.height(), j + j1 - matrix_size / 2);
                    for (int32_t i2 = 0; i2 < 4; i2++) {
                        sum[i2] += matrix[j1 * matrix_size + i1] * image.bits()[ y * image.width() * 4 + x * 4 + i2];
                    }
                }
            }
            for (int32_t i1 = 0; i1 < 3; i1++) {
                filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + i1] = bound((int)sum[i1]);
            }
            filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + 3] = image.bits()[j * image.width() * 4 + i * 4 + 3];
        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    return filtered_image;
}

QImage FiltersCluster::SimpleBlur(QImage image) {
    float matrix[9] = {0, 1.0/6.0, 0, 1.0/6.0, 2.0/6.0, 1.0/6.0, 0, 1.0/6.0, 0};
    return Blur(image, matrix, 3);
}

QImage FiltersCluster::BoxBlur(QImage image) {
    float matrix[9] = {1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0};
    return Blur(image, matrix, 3);
}

QImage FiltersCluster::GaussianBlur(QImage image) {
    float matrix[9] = {1.0/16.0, 2.0/16.0, 1.0/16.0, 2.0/16.0, 4.0/16.0, 2.0/16.0, 1.0/16.0, 2.0/16.0, 1.0/16.0};
    return Blur(image, matrix, 3);
}

QImage FiltersCluster::GammaCorrection(QImage image, float gamma) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    for (int32_t j = 0; j < filtered_image.height(); j++) {
        for (int32_t i = 0; i < filtered_image.width(); i++) {
            int byte = j * image.bytesPerLine() + i * image.depth() / 8;
            for (int32_t k = 0; k < 3; ++k) {
                filtered_image.bits()[byte + k] = 255.0 * std::pow(image.bits()[byte + k], gamma) / std::pow(255, gamma);
            }
            filtered_image.bits()[byte + 3] = image.bits()[byte + 3];
        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    return filtered_image;
}

QImage FiltersCluster::Sharpness(QImage image) {
    float matrix[9] = {0.0, -1.0,  0.0, -1.0,  5.0, -1.0, 0.0, -1.0,  0.0};
    return Blur(image, matrix, 3);
}

QImage FiltersCluster::Watercolor(QImage image, int32_t radius) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    std::vector<int32_t> local_matrix_s[4];
//    int32_t** local_matrix;
//    local_matrix = (int32_t**)malloc(4 * sizeof(int*));
//    for (int32_t i = 0; i < 4; i++) {
//        local_matrix[i] = (int32_t*)malloc(radius * 4 * radius * sizeof(int));
//    }
    for (int32_t j = 0; j < filtered_image.height(); j++) {
        for (int32_t i = 0; i < filtered_image.width(); i++) {
            int32_t index = 0;
            for (int32_t k = -radius; k < radius; ++k) {
                for (int32_t l = -radius; l < radius; ++l) {
                    int x = invert_b(image.width(), i + l);
                    int y = invert_b(image.height(), j + k);
                    for (int32_t i1 = 0; i1 < 4; i1++) {
                        local_matrix_s[i1].push_back(image.bits()[y * 4 * image.width() + x * 4 + i1]);
                        //local_matrix[i1][index] = image.bits()[y * 4 * image.width() + x * 4 + i1];
                    }
                    index++;
                }
            }
            for (int32_t i1 = 0; i1 < 4; i1++) {
                auto mid = local_matrix_s[i1].begin() + (local_matrix_s[i1].end() - local_matrix_s[i1].begin()) / 2;
                std::nth_element(local_matrix_s[i1].begin(), mid, local_matrix_s[i1].end());
//                auto mid = local_matrix[i1][radius * 2 * radius];
//                std::nth_element(&local_matrix[i1][0], &mid, &local_matrix[i1][i1 + radius * 4 * radius]);
                filtered_image.bits()[j * filtered_image.width() * 4 + i * 4 + i1] = *mid;
                local_matrix_s[i1].clear();
                //memset(local_matrix[i1], 0 , radius * 4 * radius);
            }
        }
        emit alertProgress(round((double)j / (double)image.height() * 100));
    }
    return filtered_image;
}
