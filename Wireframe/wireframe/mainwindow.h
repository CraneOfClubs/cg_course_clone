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
public slots:
    void showWireDialog();
    void help();
private:
    int32_t _grid_width = 5;
    int32_t _grid_height = 7;
    int32_t _segments_count = 8;
    float _min_t= 0.5;
    float _max_t= 1;
    float _r_min= 0;
    float _r_max= 3.14159265358979323846;
    Ui::MainWindow *ui;
    BsplineView *bsview;
};

#endif // MAINWINDOW_H
