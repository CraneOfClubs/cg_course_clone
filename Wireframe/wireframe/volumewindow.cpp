#include "volumewindow.h"

VolumeWindow::VolumeWindow(QWidget *parent)
{
    _parent = parent;
    _viewwidget = new VolumeView(this);
    this->setMaximumHeight(600);
    this->setMinimumHeight(600);
    this->setMaximumWidth(800);
    this->setMinimumWidth(800);
    _viewwidget->setGeometry(5,5,795,595);
    MainWindow *mw = dynamic_cast<MainWindow*>(_parent);
}
