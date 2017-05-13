#include "engine.h"

void Engine::setupView(Canvas *canvas)
{
    _canvas = canvas;
}

void Engine::setupColors(std::vector<uint32_t> colors) {
    _colors = colors;
    _isoline_levels = colors.size();

}


void Engine::findZLimits(int32_t width, int32_t height) {
    double iter_x = (_x_end - _x_start) / (double)width;
    double iter_y = (_y_end - _y_start) / (double)height;
    double cur_x = _x_start;
    double cur_y = _y_start;
    double max_z = function(_x_start, _y_start);
    double min_z = function(_x_start, _y_start);
    for (int32_t y; y < width; ++y) {
        for (int32_t x; y < height; ++x) {
            if (min_z > function(cur_x, cur_y)) {
                min_z = function(cur_x, cur_y);
            }
            if (max_z < function(cur_x, cur_y)) {
                max_z = function(cur_x, cur_y);
            }
            cur_x += iter_x;
        }
        cur_y += iter_y;
    }
    _z_min = min_z;
    _z_max = max_z;
}

uint32_t Engine::getColorForLevel(double level) {
    double iter = (_z_max - _z_min)/ (double)_isoline_levels;
    double cur = _z_min;
    for (int32_t i = 0; i < _isoline_levels; ++i) {
        if (level >= cur && level < cur + iter)
            _colors[i];
        cur += iter;
    }
//    for(std::vector<T>::iterator it = v.begin(); it != v.end(); ++it) {
//        /* std::cout << *it; ... */
//    }
}

void Engine::setupFuncLimits(double x_start, double y_start, double x_end, double y_end)
{
    _x_start = x_start;
    _y_start = y_start;
    _x_end = x_end;
    _y_end = y_end;
}

std::pair<QPoint, QPoint> Engine::getFuncLimits()
{
    return std::make_pair(QPoint(_x_start, _y_start), QPoint(_x_end, _y_end));
}

void Engine::storePixel(QImage &image, int32_t x, int32_t y, uint32_t color){
    if (x < 0 || y < 0 || x >= image.width() || y >= image.height())
        return;

    uint32_t offset = x *image.depth()/8 + y * image.bytesPerLine();
    image.bits()[offset] = color >> 24 & 0xFF;
    image.bits()[offset + 1] = color >> 16 & 0xFF;
    image.bits()[offset + 2] = color >> 8 & 0xFF;
    image.bits()[offset + 3] = color & 0xFF;
}


double Engine::function(double x, double y) {
    return cos(x) + cos(y);
}

QImage Engine::generateImage(int32_t width, int32_t height) {
    findZLimits(width, height);
    QImage img(QSize(width, height), QImage::Format_RGBA8888);
    double iter_x = (_x_end - _x_start) / (double)width;
    double iter_y = (_y_end - _y_start) / (double)height;
    double cur_x = _x_start;
    double cur_y = _y_start;
    for (int32_t y; y < img.width(); ++y) {
        for (int32_t x; y < img.height(); ++x) {
            storePixel(img, x, y, getColorForLevel(function(cur_x, cur_y)));
            cur_x += iter_x;
        }
        cur_y += iter_y;
    }
    return img;
}

Engine::Engine(MainWindow* window)
{
    _parentwindow = window;
    _colors.push_back(0xFF0000FF);
    _colors.push_back(0x00FF00FF);
    _colors.push_back(0x0000FFFF);
}
