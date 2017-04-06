//#pragma pack(1)
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <canvas.h>
#include <engine.h>
#include <newgamewindow.h>
#include <helpwindow.h>
#include <settingswindow.h>
#include <QImage>
#include <QTimer>
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QDialog>
#include <QScrollBar>
#include <QMenu>
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
    Engine *engine;
protected:
    virtual void resizeEvent(QResizeEvent* event);

protected slots:
    void toggleImpacts();
private:
    Ui::MainWindow *ui;
    Canvas *wgt;
    QTimer *tmr;
    QTimer *refresh_timer;
    QTimer *step_timer;

    //--------------------UI---------------------
    QPushButton* m_button;
    QPushButton* save_button;
    QPushButton* step_button;
    QPushButton* auto_button;
    QPushButton* new_button;
    QPushButton* help_button;
    QPushButton* settings_button;
    QCheckBox* box_debug;
    QCheckBox* box_impacts;
    QLabel* x_offset_label;
    QLabel* y_offset_label;
    QLabel* x_canvas_size_label;
    QLabel* y_canvas_size_label;
    QLabel* x_mouseover_label;
    QLabel* y_mouseover_label;
    QSpinBox* timeSpinBox;
    QScrollBar* vertScroll;
    QScrollBar* horisScroll;

    QMenu *fileMenu;
    QMenu *gameMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;

    QAction *stepAct;
    QAction *autoAct;
    QAction *settingsAct;
    QAction *impactsAct;

    QAction *helpAct;
private slots:
      void handleHelpButton();
      void handleSettingsButton();
      void handleButton();
      void handleStepTimer();
      void handleAutoButton();
      void handleStepButton();
      void handleQuit();
      void updateTime();
      void handleDebugBox(int toggle);
      void delayedRefresh();
      void handleShowImpacts(int toggle);
      void handleSaveButton();
      void handleNewButton();
      void handleVertScroll();
      void handleHorisScroll();
};

#endif // MAINWINDOW_H
