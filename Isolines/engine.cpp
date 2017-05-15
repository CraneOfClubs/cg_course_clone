#include "engine.h"

void Engine::setupView(Canvas *canvas)
{
    _canvas = canvas;
}

void Engine::setupColors(std::vector<uint32_t> colors) {
    _colors = colors;
    _isoline_levels = colors.size();

}

std::vector<uint32_t> Engine::getAllColors() {
    return _colors;
}

int32_t Engine::getAmountOfLevels() {
    return _isoline_levels;
}

double Engine::getValueForPixel(int32_t width, int32_t height, int32_t _x, int32_t _y) {
    double iter_x = (_x_end - _x_start) / (double)width;
    double iter_y = (_y_end - _y_start) / (double)height;
    double cur_x = _x_start;
    double cur_y = _y_start;
    for (int32_t y = 0; y < height; ++y) {
        for (int32_t x = 0; x < width; ++x) {
            if (x == _x && _y == y) {
                return function(cur_x, cur_y);
            }
            cur_x += iter_x;
        }
        cur_x = _x_start;
        cur_y += iter_y;
    }
}

void Engine::findZLimits(int32_t width, int32_t height) {
    double iter_x = (_x_end - _x_start) / (double)width;
    double iter_y = (_y_end - _y_start) / (double)height;
    double cur_x = _x_start;
    double cur_y = _y_start;
    double max_z = function(_x_start, _y_start);
    double min_z = function(_x_start, _y_start);
    for (int32_t y = 0; y < height; ++y) {
        for (int32_t x = 0; x < width; ++x) {
            if (min_z > function(cur_x, cur_y)) {
                min_z = function(cur_x, cur_y);
            }
            if (max_z < function(cur_x, cur_y)) {
                max_z = function(cur_x, cur_y);
            }
            cur_x += iter_x;
        }
        cur_x = _x_start;
        cur_y += iter_y;
    }
    _z_min = min_z;
    _z_max = max_z;
}

std::pair<double, double> Engine::getZLimits()
{
    return std::make_pair(_z_min, _z_max);
}

uint32_t Engine::getColorForLevel(double level) {
    double iter = (_z_max - _z_min)/ (double)_isoline_levels;
    double cur = _z_min;
    if (level == _z_max) {
        return _colors[_colors.size()- 1];
    }
    if (level == _z_min) {
        return _colors[0];
    }
    for (int32_t i = 0; i < _isoline_levels; ++i) {
        if (level > cur && level <= cur + iter)
            return _colors[i];
        cur += iter;
    }
}

std::vector<double> Engine::getAllLevels() {
    double iter = (_z_max - _z_min)/ (double)_isoline_levels;
    double cur = _z_min;
    std::vector<double> buf;
    for (int32_t i = 0; i < _isoline_levels; ++i) {
        cur += iter;
        buf.push_back(cur);
    }
    return buf;
}

void Engine::setupFuncLimits(double x_start, double y_start, double x_end, double y_end)
{
    _x_start = x_start;
    _y_start = y_start;
    _x_end = x_end;
    _y_end = y_end;
}

std::pair<std::pair<double, double>, std::pair<double, double>> Engine::getFuncLimits()
{
    return std::make_pair(std::make_pair(_x_start, _y_start), std::make_pair(_x_end, _y_end));
}

void Engine::storePixel(QImage *image, int32_t x, int32_t y, uint32_t color){
    if (x < 0 || y < 0 || x >= image->width() || y >= image->height())
        return;

    uint32_t offset = x *image->depth()/8 + y * image->bytesPerLine();
    image->bits()[offset] = color >> 24 & 0xFF;
    image->bits()[offset + 1] = color >> 16 & 0xFF;
    image->bits()[offset + 2] = color >> 8 & 0xFF;
    image->bits()[offset + 3] = color & 0xFF;
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
    for (int32_t y = 0; y < img.height(); ++y) {
        for (int32_t x = 0; x < img.width(); ++x) {
            double buf = function(cur_x, cur_y);
            uint32_t color = getColorForLevel(buf);
            storePixel(&img, x, y, color);
            cur_x += iter_x;
        }
        cur_x = _x_start;
        cur_y += iter_y;
    }
    return img;
}

std::vector<std::pair<QPoint, QPoint>> Engine::handleSingleCell(std::vector<std::pair<QPoint, double>> &edges, double value) {
    std::partial_sort(edges.begin(), edges.begin() + 1, edges.end(), [] (const std::pair<QPoint, double> &v1, const std::pair<QPoint, double> &v2) -> bool {
        return v1.second > v2.second;
    });
    std::pair<QPoint, double> point = edges[0];
    QPoint start(point.first.x(), 0);
    QPoint end(0, point.first.y());
    return iterateLine(edges, value, start, end);
}

std::vector<std::pair<QPoint, QPoint>> Engine::handleTripleCell(std::vector<std::pair<QPoint, double>> &edges, double value) {
    std::partial_sort(edges.begin(), edges.begin() + 3, edges.end(), [] (const std::pair<QPoint, double> &v1, const std::pair<QPoint, double> &v2) -> bool {
        return v1.second < v2.second;
    });
    QPoint start(edges[0].first.x(), 0);
    QPoint end(0, edges[0].first.y());
    return iterateLine(edges, value, start, end);
}

std::vector<std::pair<QPoint, QPoint>> Engine::iterateLine(std::vector<std::pair<QPoint, double>> &edges, double iso_level, QPoint start, QPoint end) {
    for (auto it = edges.begin() + 1; it < edges.end(); ++it) {
        if (edges[0].first.x() == it->first.x()) {
            int32_t offset = edges[0].second > it->second ? edges[0].first.y() : it->first.y();
            int32_t sign = offset == std::max(edges[0].first.y(), it->first.y()) ? -1 : 1;
            start.setY(sign * std::fabs(iso_level - std::max(edges[0].second, it->second)) * std::abs(edges[0].first.y() - it->first.y()) / std::fabs(edges[0].second - it->second) + offset);
        }
        if (edges[0].first.y() == it->first.y()) {
            int32_t offset = edges[0].second > it->second ? edges[0].first.x() : it->first.x();
            int32_t sign = offset == std::max(edges[0].first.x(), it->first.x()) ? -1 : 1;
            end.setX(sign * std::fabs(iso_level - std::max(edges[0].second, it->second)) * std::abs(edges[0].first.x() - it->first.x()) / std::fabs(edges[0].second - it->second) + offset);
        }
    }
    return std::vector<std::pair<QPoint, QPoint>>{std::make_pair(start, end)};
}

void Engine::setUserIsoline(double value, bool turn_on)
{
    _user_isoline_level = value;
    _user_isoline = turn_on;
}

bool Engine::isUserIsolineActive() {
    return _user_isoline;
}

double Engine::getUserIsolineLevel() {
    return _user_isoline_level;
}

int32_t Engine::getSign(int32_t offset, int32_t x, int32_t y){
    if (offset == std::max(x, y))
        return -1;
    else
        return 1;
}

std::vector<std::pair<QPoint, QPoint>> Engine::calcDiagonal(std::vector<std::pair<QPoint, double>> &edges, double level, bool higher) {
    if (higher){
        QPoint start_first(edges[0].first.x(), 0);
        QPoint end_first(0, edges[1].first.y());
        QPoint start_second(edges[1].first.x(), 0);
        QPoint end_second(0, edges[0].first.y());
        if (edges[0].first.x() != edges[2].first.x()) {
            std::swap(edges[2], edges[3]);
        }
        int32_t offset = edges[1].first.x();
        int32_t sign = getSign(offset, edges[1].first.x(), edges[2].first.x());
        end_first.setX(offset + sign * std::fabs(level - edges[1].second) * std::abs(edges[1].first.x() - edges[2].first.x()) / std::fabs(edges[1].second - edges[2].second));
        offset = edges[0].first.y();
        sign = getSign(offset, edges[0].first.y(), edges[2].first.y());
        start_first.setY(offset + sign * std::fabs(level - edges[0].second) * std::abs(edges[0].first.y() - edges[2].first.y()) / std::fabs(edges[0].second - edges[2].second));
        offset = edges[0].first.x();
        sign = getSign(offset, edges[0].first.x(), edges[3].first.x());
        end_second.setX(offset + sign * std::fabs(level - edges[0].second) * std::abs(edges[3].first.x() - edges[0].first.x()) / std::fabs(edges[0].second - edges[3].second));
        offset = edges[1].first.y();
        sign = getSign(offset, edges[1].first.y(), edges[3].first.y());
        start_second.setY(offset + sign * std::fabs(level - edges[1].second) * std::abs(edges[1].first.y() - edges[3].first.y()) / std::fabs(edges[1].second - edges[3].second));
        return std::vector<std::pair<QPoint, QPoint>>{std::make_pair(start_first, end_first), std::make_pair(start_second, end_second)};
    } else {
        QPoint start_first(edges[0].first.x(), 0);
        QPoint end_first(0, edges[0].first.y());
        QPoint start_second(edges[1].first.x(), 0);
        QPoint end_second(0, edges[1].first.y());
        if (edges[0].first.x() != edges[2].first.x()) {
            std::swap(edges[2], edges[3]);
        }
        int32_t offset = edges[0].first.x();
        int32_t sign = getSign(offset, edges[0].first.x(), edges[3].first.x());
        end_first.setX(offset + sign * std::fabs(level - edges[0].second) * std::abs(edges[0].first.x() - edges[3].first.x()) / std::fabs(edges[0].second - edges[3].second));
        offset = edges[0].first.y();
        sign = getSign(offset, edges[0].first.y(), edges[2].first.y());
        start_first.setY(offset + sign * std::fabs(level - edges[0].second) * std::abs(edges[0].first.y() - edges[2].first.y()) / std::fabs(edges[0].second - edges[2].second));
        offset = edges[1].first.x();
        sign = getSign(offset, edges[1].first.x(), edges[2].first.x());
        end_second.setX(offset + sign * std::fabs(level - edges[1].second) * std::abs(edges[1].first.x() - edges[2].first.x()) / std::fabs(edges[1].second - edges[2].second));
        offset = edges[1].first.y();
        sign = getSign(offset, edges[1].first.y(), edges[3].first.y());
        start_second.setY(offset + sign * std::fabs(level - edges[1].second) * std::abs(edges[1].first.y() - edges[3].first.y()) / std::fabs(edges[1].second - edges[3].second));
        return std::vector<std::pair<QPoint, QPoint>>{std::make_pair(start_first, end_first), std::make_pair(start_second, end_second)};
    }
}

std::vector<std::pair<QPoint, QPoint>> Engine::calcComplicatedMesh(std::vector<std::pair<QPoint, double>> &edges, double level, double median) {
    std::partial_sort(edges.begin(), edges.begin() + 2, edges.end(), [] (const std::pair<QPoint, double> &v1, const std::pair<QPoint, double> &v2) -> bool {
        return v1.second > v2.second;
    });
        if (edges[0].first.x() == edges[1].first.x()) {
            QPoint start(0, edges[0].first.y());
            QPoint end(0, edges[1].first.y());
            if (edges[0].first.y() != edges[2].first.y()) {
                std::swap(edges[2], edges[3]);
            }
            int32_t offset = edges[0].first.x();
            int32_t sign = getSign(offset, edges[0].first.x(), edges[2].first.x());
            start.setX(offset + sign * std::fabs(level - edges[0].second) * std::abs(edges[0].first.x() - edges[2].first.x()) / std::fabs(edges[0].second - edges[2].second));
            offset = edges[1].first.x();
            sign = getSign(offset, edges[1].first.x(), edges[3].first.x());
            end.setX(offset + sign * std::fabs(level - edges[1].second) * std::abs(edges[1].first.x() - edges[3].first.x()) / std::fabs(edges[1].second - edges[3].second));
            return std::vector<std::pair<QPoint, QPoint>>{std::make_pair(start, end)};
        } else if (edges[0].first.y() == edges[1].first.y()) {
            QPoint start(edges[0].first.x(), 0);
            QPoint end(edges[1].first.x(), 0);
            if (edges[0].first.x() != edges[2].first.x()) {
                std::swap(edges[2], edges[3]);
            }
            int32_t offset = edges[0].first.y();
            int32_t sign = getSign(offset, edges[0].first.y(), edges[2].first.y());
            start.setY(offset + sign * std::fabs(level - edges[0].second) * std::abs(edges[0].first.y() - edges[2].first.y()) / std::fabs(edges[0].second - edges[2].second));
            offset = edges[1].first.y();
            sign = getSign(offset, edges[1].first.y(), edges[3].first.y());
            end.setY(offset + sign * std::fabs(level - edges[1].second) * std::abs(edges[1].first.y() - edges[3].first.y()) / std::fabs(edges[1].second - edges[3].second));
            return std::vector<std::pair<QPoint, QPoint>>{std::make_pair(start, end)};
        } else if (median > level){
            return calcDiagonal(edges, level, true);
        } else {
            return calcDiagonal(edges, level, false);
        }
    }

Engine::Engine(MainWindow* window)
{
    _parentwindow = window;
    _colors.push_back(0xFF0000FF);
    _colors.push_back(0x00FF00FF);
    _colors.push_back(0x0000FFFF);
    setupColors(_colors);
}
