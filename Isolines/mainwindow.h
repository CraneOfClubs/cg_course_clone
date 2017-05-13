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

class Canvas;
class Engine;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool _inited = false;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    Engine *engine;
    Engine *getEngine();
public slots:
    void closeIsolines();
private:
    Canvas* canvas;
    Ui::MainWindow *ui;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QAction *exitAction;
    QToolBar* toolbar;
    void createMenu();
    QToolBar *createToolBar();
};

#endif // MAINWINDOW_H
