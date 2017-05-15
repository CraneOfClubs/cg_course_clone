#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QVBoxLayout>
#include <QMainWindow>
#include <canvas.h>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <stdint.h>
#include <engine.h>
#include <isolegend.h>
#include <settingswindget.h>

class SettingsWindget;
class IsoLegend;
class Canvas;
class Engine;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool _drawisolines = 0;
    bool _drawgrid = 0;
    uint32_t grid_cols = 40;
    uint32_t grid_rows = 40;
    bool _inited = false;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    Engine *engine;
    Engine *getEngine();
    void resizeLegend();
    Canvas *canvas;
    IsoLegend *legend;
public slots:
    void closeIsolines();
    void drawIsolines();
    void drawGrid();
    void handleSettings();
    void openFile();
    void saveToFile();
private:
    Ui::MainWindow *ui;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QAction *exitAction;
    QToolBar* toolbar;
    void createMenu();
    QToolBar *createToolBar();
};

#endif // MAINWINDOW_H
