#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>

class SettingWidget : public QWidget
{
    Q_OBJECT
public:

    QLabel* firstControlName;
    QLabel* secondControlName;
    QLabel* thirdControlName;

    QSlider* firstControlSlider;
    QSlider* secondControlSlider;
    QSlider* thirdControlSlider;

    QSpinBox* firstControlSpinBox;
    QSpinBox* secondControlSpinBox;
    QSpinBox* thirdControlSpinBox;

    SettingWidget(QWidget *parent = 0);

    SettingWidget(QWidget *parent, int32_t size_x, int32_t size_y);
    void setFirstControl(QString name, double start_pos, double end_pos, double step_amount, int position, bool visible, bool fp);
    void setThirdControl(QString name, double start_pos, double end_pos, double step_amount, int position, bool visible, bool fp);
    void setSecondControl(QString name, double start_pos, double end_pos, double step_amount, int position, bool visible, bool fp);
    double getFirstControl();
    double getSecondControl();
    double getThirdControl();

private:
        double first_fp = false;
        double second_fp = false;
        double third_fp = false;

public slots:
        void handleFirstControlSlider(int val);
        void handleSecondControlSlider(int val);
        void handleThirdControlSlider(int val);
protected:

};

#endif // SETTINGWIDGET_H
