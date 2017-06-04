#ifndef VOLUMEVIEW_H
#define VOLUMEVIEW_H
#include <QMainWindow>
#include <QWidget>
#include <drawingarea.h>
#include <curvedobject.h>
#include <QMatrix4x4>
#include <transformutils.h>

class VolumeView : public DrawingArea
{
public:
    TransformUtils transform;
    CurvedObject _object;
    std::vector<CurvedObject> _objects;
    VolumeView(QWidget *parent);
    void drawFile(std::vector<Line> file, uint32_t color);
    void drawBoundingBox(float coord);
    void drawLine(QVector3D from, QVector3D to, uint32_t color);
    void redraw();
    void loadObject(CurvedObject obj);
    void loadObjects(std::vector<CurvedObject> objects);
protected:
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    double const pi = 3.14159265358979323846;
    bool _left_key_pressed = false;
    QPoint _mouse_start;
    float _clip_near = 0;
    float _clip_far = 5;
    int32_t _rect_width = 10;
    int32_t _rect_height = 10;
    QMatrix4x4 _current_rotation;
    QMatrix4x4 _current_perspective;
    QMatrix4x4 _current_camera;
    bool _objectloaded = false;
};

#endif // VOLUMEVIEW_H
