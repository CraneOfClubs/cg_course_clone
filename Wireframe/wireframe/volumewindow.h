#ifndef VOLUMEWINDOW_H
#define VOLUMEWINDOW_H
#include <QDialog>
#include <QWidget>
#include <mainwindow.h>
#include <volumeview.h>

class MainWindow;

class VolumeWindow : public QDialog
{
    Q_OBJECT
public:
    VolumeView * _viewwidget;
    QWidget *_parent;
    VolumeWindow(QWidget *parent);
    void loadObject(CurvedObject obj);
    void loadObjects(std::vector<CurvedObject> _objects);
private:

};

#endif // VOLUMEWINDOW_H
