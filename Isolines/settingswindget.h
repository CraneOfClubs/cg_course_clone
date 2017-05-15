#ifndef SETTINGSWINDGET_H
#define SETTINGSWINDGET_H
#include <QDialog>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <mainwindow.h>
#include <QColorDialog>

class MainWindow;
class SettingsWindget : public QDialog
{
    Q_OBJECT
public slots:
    void saveSettings();
    void addNewColor();
    void removeColors();
private:
    QWidget *_parent;
    QGridLayout * grid;
    QGridLayout * ugrid;
    QDoubleSpinBox *x_start;
    QDoubleSpinBox *y_start;
    QDoubleSpinBox *x_end;
    QDoubleSpinBox *y_end;
    QSpinBox *columns;
    QSpinBox *rows;
    QListWidget * _color_widget;
    QPushButton *_apply;
    QPushButton *_add_color;
    QPushButton *_remove_color;
public:
    SettingsWindget(QWidget *parent = 0);
};

#endif // SETTINGSWINDGET_H
