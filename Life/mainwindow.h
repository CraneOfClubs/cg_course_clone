//#pragma pack(1)
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <canvas.h>
#include <engine.h>
#include <newgamewindow.h>
#include <helpwindow.h>
#include <QImage>
#include <QTimer>
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QDialog>
#define X_CANVAS_OFFSET 130
#define Y_CANVAS_OFFSET 50


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void refreshDebugInfo();
    ~MainWindow();   
    void callNewGame(int32_t xsize, int32_t ysize);
protected:
    virtual void resizeEvent(QResizeEvent* event);

private:
    Ui::MainWindow *ui;
    Canvas *wgt;
    QTimer *tmr;
    QTimer *refresh_timer;
    QTimer *step_timer;
    Engine *engine;
    //--------------------UI---------------------
    QPushButton* m_button;
    QPushButton* save_button;
    QPushButton* step_button;
    QPushButton* auto_button;
    QPushButton* new_button;
    QPushButton* help_button;
    QCheckBox* box_debug;
    QCheckBox* box_impacts;
    QLabel* x_offset_label;
    QLabel* y_offset_label;
    QLabel* x_canvas_size_label;
    QLabel* y_canvas_size_label;
    QLabel* x_mouseover_label;
    QLabel* y_mouseover_label;
    QSpinBox* timeSpinBox;


private slots:
      void handleHelpButton();
      void handleButton();
      void handleStepTimer();
      void handleAutoButton();
      void handleStepButton();
      void updateTime();
      void handleDebugBox(int toggle);
      void delayedRefresh();
      void handleShowImpacts(int toggle);
      void handleSaveButton();
      void handleNewButton();
};

#endif // MAINWINDOW_H
