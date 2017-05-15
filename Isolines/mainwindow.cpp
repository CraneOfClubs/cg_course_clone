#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    centralWidget = new QWidget(this);
    engine = new Engine(this);

    mainLayout = new QVBoxLayout(centralWidget);

    this->setCentralWidget( centralWidget );
    canvas = new Canvas(this);
    engine->setupView(canvas);
    engine->setupFuncLimits(-5, -5 , 5 , 5);
    mainLayout->addWidget(canvas);
    legend = new IsoLegend(this);
    mainLayout->addWidget(legend);

    createMenu();
    setMenuBar(menuBar);
    addToolBar(Qt::TopToolBarArea, createToolBar());
    this->show();
    _inited = true;
    this->setGeometry(10, 10, 600, 600);
    engine->findZLimits(canvas->width(), canvas->height());
    legend->recalcImage();
}

void MainWindow::resizeLegend() {
    ;
}

Engine* MainWindow::getEngine() {
    return engine;
}

void MainWindow::closeIsolines() {
    this->close();
}

void MainWindow::drawGrid() {
    _drawgrid = !_drawgrid;
    canvas->redraw();
}

void MainWindow::drawIsolines() {
    _drawisolines = !_drawisolines;
    canvas->redraw();
}

void MainWindow::handleSettings()
{
    SettingsWindget *settings = new SettingsWindget(this);
    settings->exec();
}

void MainWindow::saveToFile()
{
    SettingsWindget *settings = new SettingsWindget(this);
    settings->exec();
}

void MainWindow::openFile()
{
    SettingsWindget *settings = new SettingsWindget(this);
    settings->exec();
}



void MainWindow::createMenu()
{
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    exitAction = fileMenu->addAction(tr("E&xit"));
    menuBar->addMenu(fileMenu);

    //connect(exitAction, SIGNAL(triggered()), this, SLOT(accept()));
}

QToolBar* MainWindow::createToolBar()
{
    QToolBar* ptb = new QToolBar(centralWidget);
    ptb->addAction(QIcon(":/img/open.png"), tr("Open File"), this, SLOT(closeIsolines()));
    ptb->addAction(QIcon(":/img/save.jpg"), tr("Save to file"), this, SLOT(drawIsolines()));
    ptb->addSeparator();
    ptb->addAction(QIcon(":/img/img/new.png"), tr("New Isolines"), this, SLOT(closeIsolines()));
    ptb->addAction(QIcon(":/img/lines.png"), tr("Draw Isolines"), this, SLOT(drawIsolines()));
    ptb->addAction(QIcon(":/img/grid.png"), tr("Draw Grid"), this, SLOT(drawGrid()));
    ptb->actions()[1]->setCheckable(4);
    ptb->actions()[1]->setCheckable(5);
    ptb->addSeparator();
    ptb->addAction(QIcon(":/img/settings.png"), tr("Settings"), this, SLOT(handleSettings()));
    return ptb;
}

MainWindow::~MainWindow()
{
    delete ui;
}
