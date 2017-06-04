#ifndef BSPLINEVIEW_H
#define BSPLINEVIEW_H

#include <QMainWindow>
#include <QWidget>
#include <drawingarea.h>
#include <curvedobject.h>

class BsplineView : public DrawingArea
{
public:
    std::vector<CurvedObject> _objects;
    CurvedObject _object;
    explicit BsplineView(QWidget *parent);
    void loadObject(CurvedObject *object);
    void drawObject(CurvedObject cur_obj);
    float detectScale(std::vector<QPointF> &knots);
    CurvedObject getObject();
private:
    float def_scale = 0.9;
};

#endif // BSPLINEVIEW_H
