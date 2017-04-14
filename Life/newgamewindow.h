#ifndef NEWGAMEWINDOW_H
#define NEWGAMEWINDOW_H
#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <mainwindow.h>

class MainWindow;

namespace Ui {
class NewGameWindow;
}

class NewGameWindow : public QDialog
{
    Q_OBJECT

public:
    NewGameWindow();
    NewGameWindow(int32_t xpos, int32_t ypos, MainWindow *parent);
private:
    QSpinBox* xSpinBox;
    QSpinBox* ySpinBox;
    QLabel* xLabel;
    QLabel* yLabel;
    MainWindow *_parent;
    QPushButton* save_button;
private slots:
      void handleButton();
};

#endif // NEWGAMEWINDOW_H
