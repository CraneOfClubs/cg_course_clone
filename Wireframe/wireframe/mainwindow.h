#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <drawingarea.h>
#include <bsplineview.h>
#include <curvedobject.h>
#include <volumewindow.h>

class VolumeWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QToolBar *createToolBar();
    CurvedObject* start_obj;
    CurvedObject* start_obj1;
public slots:
    void showWireDialog();
    void help();
private:
    VolumeWindow *volumecanvas;
    int32_t _grid_width = 10;
    int32_t _grid_height = 12;
    int32_t _segments_count = 5;
    float _min_t= 0;
    float _max_t= 1;
    float _r_min= 0;
    float _r_max= 2 * 3.14185265358979323946;
    Ui::MainWindow *ui;
    BsplineView *bsview;
};

#endif // MAINWINDOW_H
