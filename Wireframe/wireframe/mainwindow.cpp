#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <helpwindow.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    bsview = new BsplineView(this);
    this->setGeometry(500,500,800,600);
    this->setMaximumHeight(600);
    this->setMinimumHeight(600);
    this->setMaximumWidth(800);
    this->setMinimumWidth(800);
    bsview->setGeometry(5,40,790,555);
    bsview->fill(0x000000FF);
    std::vector<QPointF> start_knots({QPointF(-0.1, -0.2), QPointF(-0.0, -0.18), QPointF(-0.2, 0.5), QPointF(-0.9, 0.0)});
    std::vector<QPointF> start_knots1({QPointF(0.1, 0.2), QPointF(0.0, 0.18), QPointF(0.7, -1.5), QPointF(0.8, -0.5)});
    start_obj = new CurvedObject(_grid_height, _grid_width, _segments_count, _min_t, _max_t, _r_min, _r_max, start_knots, 0xFF0000FF);
    start_obj1 = new CurvedObject(_grid_height + 10, _grid_width + 10, _segments_count, _min_t, _max_t, _r_min, _r_max, start_knots1, 0xFF00FFFF);
    bsview->loadObject(start_obj);
    bsview->loadObject(start_obj1);
    addToolBar(Qt::TopToolBarArea, createToolBar());
    this->repaint();
}

void MainWindow::showWireDialog()
{
    volumecanvas = new VolumeWindow(this);
    std::vector<CurvedObject> _objects;
    _objects.push_back(*start_obj);
    _objects.push_back(*start_obj1);
    volumecanvas->loadObjects(_objects);
    volumecanvas->exec();
}

void MainWindow::help()
{
    HelpWindow *help = new HelpWindow(this->x() + (this->width() /2), this->y()  + (this->height()/ 2));
    help->exec();
}

QToolBar* MainWindow::createToolBar()
{
    QToolBar* ptb = new QToolBar(this);
    ptb->addAction(QIcon(":/img/img/grid.png"), tr("Show Wire model"), this, SLOT(showWireDialog()));
    ptb->addSeparator();
    ptb->addAction(QIcon(":/img/img/help.png"), tr("Help"), this, SLOT(help()));
    return ptb;
}

MainWindow::~MainWindow()
{
    delete ui;
}
