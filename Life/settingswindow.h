#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H


#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QSlider>
#include <mainwindow.h>

class MainWindow;

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    SettingsWindow();
    SettingsWindow(int32_t xpos, int32_t ypos, MainWindow *parent);
private:
    QSlider* firstImpact;
    QSlider* secondImpact;
    QSlider* birthBegin;
    QSlider* birthEnd;
    QSlider* liveBegin;
    QSlider* liveEnd;
    QLabel* firstImpactLabel;
    QLabel* secondImpactLabel;
    QLabel* birthBeginLabel;
    QLabel* birthEndLabel;
    QLabel* liveBeginLabel;
    QLabel* liveEndLabel;
    MainWindow *_parent;
    QPushButton* save_button;
private slots:
    void handleButton();
    void handleFirstImpact(int val);
    void handleSecondImpact(int val);
    void handleLiveBegin(int val);
    void handleLiveEnd(int val);
    void handleBirthEnd(int val);
    void handleBirthBegin(int val);
};
#endif // SETTINGSWINDOW_H
