#include "settingwidget.h"
#include <QMessageBox>

SettingWidget::SettingWidget(QWidget *parent) : QWidget(parent)
{

}



SettingWidget::SettingWidget(QWidget *parent, int32_t size_x, int32_t size_y) : QWidget(parent)
{
    this->setGeometry(50, 400, size_x, size_y);
    setMouseTracking(true);


    firstControlName = new QLabel(this);
    secondControlName = new QLabel(this);
    thirdControlName = new QLabel(this);

    firstControlSlider = new QSlider(Qt::Horizontal,this);
    secondControlSlider = new QSlider(Qt::Horizontal,this);
    thirdControlSlider = new QSlider(Qt::Horizontal,this);

    firstControlSpinBox = new QSpinBox(this);
    secondControlSpinBox = new QSpinBox(this);
    thirdControlSpinBox = new QSpinBox(this);

    firstControlName->setVisible(false);
    secondControlName->setVisible(false);
    thirdControlName->setVisible(false);

    firstControlSlider->setVisible(false);
    secondControlSlider->setVisible(false);
    thirdControlSlider->setVisible(false);

    firstControlSpinBox->setVisible(false);
    secondControlSpinBox->setVisible(false);
    thirdControlSpinBox->setVisible(false);

    connect(firstControlSlider, SIGNAL(valueChanged(int)), this, SLOT(handleFirstControlSlider(int)));
    connect(secondControlSlider, SIGNAL(valueChanged(int)), this, SLOT(handleSecondControlSlider(int)));
    connect(thirdControlSlider, SIGNAL(valueChanged(int)), this, SLOT(handleThirdControlSlider(int)));
}

void SettingWidget::handleFirstControlSlider(int val) {
    firstControlSpinBox->setValue(firstControlSlider->value());
}

void SettingWidget::handleSecondControlSlider(int val) {
    secondControlSpinBox->setValue(secondControlSlider->value());
}

void SettingWidget::handleThirdControlSlider(int val) {
    thirdControlSpinBox->setValue(thirdControlSlider->value());
}

void SettingWidget::setFirstControl(QString name, double start_pos, double end_pos, double step_amount, int position, bool visible, bool fp)
{
    first_fp = fp;
    firstControlName->setText(name);
    firstControlName->setVisible(visible);
    if (!fp) {
        firstControlSlider->setRange(round(start_pos), round(end_pos));
        firstControlSlider->setSingleStep(round(step_amount));
        firstControlSlider->setValue(round(start_pos));
        firstControlSlider->setVisible(visible);
        firstControlSlider->setEnabled(true);

        firstControlSpinBox->setRange(round(start_pos), round(end_pos));
        firstControlSpinBox->setSingleStep(round(step_amount));
        firstControlSpinBox->setValue(round(start_pos));
        firstControlSpinBox->setVisible(visible);
    } else {
        firstControlSlider->setRange(start_pos, end_pos);
        firstControlSlider->setSingleStep(step_amount);
        firstControlSlider->setValue(start_pos);
        firstControlSlider->setVisible(visible);
        firstControlSlider->setEnabled(true);

        firstControlSpinBox->setRange(start_pos, end_pos);
        firstControlSpinBox->setSingleStep(step_amount);
        firstControlSpinBox->setValue(start_pos);
        firstControlSpinBox->setVisible(visible);
    }
    firstControlName->setGeometry(QRect(QPoint(50, 20 + position * 30), QSize(60, 20)));
    firstControlSlider->setGeometry(QRect(QPoint(110, 20 + position * 30), QSize(100, 20)));
    firstControlSpinBox->setGeometry(QRect(QPoint(220, 20 + position * 30), QSize(50, 20)));
}

void SettingWidget::setSecondControl(QString name, double start_pos, double end_pos, double step_amount, int position, bool visible, bool fp)
{
    second_fp = fp;
    secondControlName->setText(name);
    secondControlName->setVisible(visible);
    if (!fp) {
        secondControlSlider->setRange(round(start_pos), round(end_pos));
        secondControlSlider->setSingleStep(round(step_amount));
        secondControlSlider->setValue(round(start_pos));
        secondControlSlider->setVisible(visible);
        secondControlSlider->setEnabled(true);

        secondControlSpinBox->setRange(round(start_pos), round(end_pos));
        secondControlSpinBox->setSingleStep(round(step_amount));
        secondControlSpinBox->setValue(round(start_pos));
        secondControlSpinBox->setVisible(visible);
    }
    secondControlName->setGeometry(QRect(QPoint(50, 20 + position * 30), QSize(60, 20)));
    secondControlSlider->setGeometry(QRect(QPoint(110, 20 + position * 30), QSize(100, 20)));
    secondControlSpinBox->setGeometry(QRect(QPoint(220, 20 + position * 30), QSize(50, 20)));
}

double SettingWidget::getFirstControl()
{
    return firstControlSpinBox->value();
}

double SettingWidget::getSecondControl()
{
    return secondControlSpinBox->value();
}

double SettingWidget::getThirdControl()
{
    return thirdControlSpinBox->value();
}

void SettingWidget::setThirdControl(QString name, double start_pos, double end_pos, double step_amount, int position, bool visible, bool fp)
{
    third_fp = fp;
    thirdControlName->setText(name);
    thirdControlName->setVisible(visible);
    if (!fp) {
        thirdControlSlider->setRange(round(start_pos), round(end_pos));
        thirdControlSlider->setSingleStep(round(step_amount));
        thirdControlSlider->setValue(round(start_pos));
        thirdControlSlider->setVisible(visible);
        thirdControlSlider->setEnabled(true);

        thirdControlSpinBox->setRange(round(start_pos), round(end_pos));
        thirdControlSpinBox->setSingleStep(round(step_amount));
        thirdControlSpinBox->setValue(round(start_pos));
        thirdControlSpinBox->setVisible(visible);
    }
    thirdControlName->setGeometry(QRect(QPoint(50, 20 + position * 30), QSize(60, 20)));
    thirdControlSlider->setGeometry(QRect(QPoint(110, 20 + position * 30), QSize(100, 20)));
    thirdControlSpinBox->setGeometry(QRect(QPoint(220, 20 + position * 30), QSize(50, 20)));
}
