#ifndef ENGINE_H
#define ENGINE_H
#include <stdint.h>
#include <canvas.h>
#include <QPoint>
#include <vector>
#include <mainwindow.h>

class MainWindow;
class Canvas;

class Engine
{
private:
    MainWindow * _parentwindow;
    std::vector<uint32_t> _colors;
    int32_t _isoline_levels;
    double _user_isoline_level;
    double _x_start = 0;
    double _y_start = 0;
    double _x_end = 1;
    double _y_end = 1;
    double _z_min;
    double _z_max;
    Canvas *_canvas;
    void storePixel(QImage &image, int32_t x, int32_t y, uint32_t color);
    void findZLimits(int32_t width, int32_t height);
public:
    void setupView(Canvas* canvas);
    void setupFuncLimits(double x_start, double y_start, double x_end, double y_end);
    std::pair<QPoint, QPoint> getFuncLimits();
    Engine(MainWindow* window);
    double function(double x, double y);
    uint32_t getColorForLevel(double level);
    void setupColors(std::vector<uint32_t> colors);
    QImage generateImage(int32_t width, int32_t height);
};

#endif // ENGINE_H
