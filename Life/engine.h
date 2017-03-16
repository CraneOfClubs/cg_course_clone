#ifndef ENGINE_H
#define ENGINE_H
#include <map>
#include <QFileDialog>
#include <string>
#include <canvas.h>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextStream>
#include <QException>
#include "settings.h"

class Canvas;

class Engine
{
private:
    std::map<std::pair<int32_t,int32_t>, std::pair<float, bool>> *cells;
    Canvas* view;

public:
    settings _settings;
    float getImpact(int32_t i, int32_t j);
    void reCalcImpacts();
    bool step();
    bool setCell(int32_t x, int32_t y);
    int32_t columns = 30;
    int32_t rows = 30;
    Engine(Canvas* _view);
    void loadFromFile(std::string filename);
    void saveToFile(std::string filename);
    std::map<std::pair<int32_t,int32_t>, std::pair<float, bool>> getCells();
};

#endif // ENGINE_H
