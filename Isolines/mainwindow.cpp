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
    mainLayout->addWidget(canvas);
    //canvas->setGeometry(10, 10, 200, 200);

    createMenu();
    setMenuBar(menuBar);
    addToolBar(Qt::TopToolBarArea, createToolBar());
    this->show();
    _inited = true;
    //mainLayout->setAlignment(Qt::AlignTop);
}

Engine* MainWindow::getEngine() {
    return engine;
}

void MainWindow::closeIsolines() {
    this->close();
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

    ptb->addAction(QIcon(":/img/img/new.png"), tr("New Isolnes"), this, SLOT(closeIsolines()));
    ptb->addSeparator();
    return ptb;
}

MainWindow::~MainWindow()
{
    delete ui;
}
