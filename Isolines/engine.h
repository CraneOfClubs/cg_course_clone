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
    double _iso_delim = 0;
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
    void storePixel(QImage *image, int32_t x, int32_t y, uint32_t color);
    bool _user_isoline = false;
    std::vector<std::pair<QPoint, QPoint> > iterateLine(std::vector<std::pair<QPoint, double> > &edges, double value, QPoint start, QPoint end);
public:
    void findZLimits(int32_t width, int32_t height);
    std::pair<double, double> getZLimits();
    void setupView(Canvas* canvas);
    void setupFuncLimits(double x_start, double y_start, double x_end, double y_end);
    std::pair<std::pair<double, double>, std::pair<double, double>> getFuncLimits();
    Engine(MainWindow* window);
    double function(double x, double y);
    uint32_t getColorForLevel(double level);
    void setupColors(std::vector<uint32_t> colors);
    int32_t getAmountOfLevels();
    std::vector<double> getAllLevels();
    std::vector<std::pair<QPoint, QPoint> > calcComplicatedMesh(std::vector<std::pair<QPoint, double> > &edges, double level, double median);
    std::vector<uint32_t> getAllColors();
    double getValueForPixel(int32_t width, int32_t height, int32_t _x, int32_t _y);
    std::vector<std::pair<QPoint, QPoint> > handleSingleCell(std::vector<std::pair<QPoint, double> > &vertices, double value);
    std::vector<std::pair<QPoint, QPoint> > handleTripleCell(std::vector<std::pair<QPoint, double> > &edges, double value);
    int32_t getSign(int32_t offset, int32_t x, int32_t y);
    std::vector<std::pair<QPoint, QPoint> > calcDiagonal(std::vector<std::pair<QPoint, double> > &edges, double level, bool higher);
    void setUserIsoline(double value, bool turn_on);
    QImage generateImage(int32_t width, int32_t height, bool interpolate);
    uint32_t getInterpolatedColor(double val);
public slots:
    double getUserIsolineLevel();
    bool isUserIsolineActive();
};

#endif // ENGINE_H
