#ifndef CANVAS_H
#define CANVAS_H
#include <QWidget>
#include <QPainter>
#include <QtGui/QMouseEvent>
#include <QImage>
#include <QPoint>
#include <math.h>
#include <stack>
#include <engine.h>

#define _DEBUG 0
#define COS60 0.8660254037844386
#define NOT_DEF -1
#define VALID 1
#define INVALID 0


class QPaintEvent;
class Engine;

typedef struct {
    int32_t x = -10;
    int32_t y = -10;
    uint8_t valid = NOT_DEF;
} Hex_number;

class Canvas : public QWidget
{
    Q_OBJECT
public:   
    Canvas(QWidget * parent = 0, int32_t size_x = 50, int32_t size_y = 50);
    void bindEngine(Engine *engine);
    void resizeAndMove(QRect rect);
    void setPixel(int32_t x, int32_t y, uint32_t color);
    void storePixel(int32_t x, int32_t y, uint32_t color);
    void fillCanvas();
    void drawLine(int x1, int y1, int x2, int y2, uint32_t thickness, uint32_t color);
    void storePoint(uint32_t x, uint32_t y, uint32_t color, uint16_t thickness);
    void fillWithGex(uint16_t sizeOfGex, uint16_t thickness, std::map<std::pair<int32_t,int32_t>, std::pair<double, bool>> cells, QPoint cell_amount);
    uint32_t getPixelColor(int32_t x, int32_t y);
    void fillAt(int32_t x, int32_t y, uint32_t color);
    void floodFillScanlineStack(int x_INVERTED, int y_INVERTED, uint32_t newColor, uint32_t oldColor);
    QPoint getDisplayOffset();
    QPoint getCanvasSize();
    int32_t getSizeOfGex();
    int16_t getThickness();
    Hex_number getSelectedHexagon(int x, int y);
    int32_t gridWidth = 0;
    int32_t halfWidth = 0;
    int32_t gridHeight = 0;
    int32_t halfHeight = 0;
    bool showImpacts = false;

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    void fillGexes(std::map<std::pair<int32_t,int32_t>, std::pair<double, bool>> cells);
    Engine * engine;
    struct Mouse_states {
        bool lmb = 0;
        bool rmb = 0;
        bool mmb = 0;
        int32_t prev_x = -10;
        int32_t prev_y = -10;

    } mouse_states;
    Hex_number prev_hex;
    void brLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color);
    int32_t display_offset_x = 0;
    int32_t display_offset_y = 0;

    int32_t sizeOfGex = 20;
    int16_t thickness = 1;

#pragma pack(push,1)
QImage* image;
#pragma pack(pop)
    int32_t anchor_x;
    int32_t anchor_y;
    int32_t size_x;
    int32_t size_y;
};

#endif // CANVAS_H
