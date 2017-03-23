#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    refresh_timer = new QTimer();
    refresh_timer->setInterval(20); //100 fps?
    refresh_timer->setSingleShot(true);

    tmr = new QTimer();
    tmr->setInterval(400); // Задаем интервал таймера
    tmr->start(); // Запускаем таймер

    step_timer = new QTimer();

    ui->setupUi(this);
    wgt = new Canvas(this);
    wgt->resizeAndMove(QRect(X_CANVAS_OFFSET,Y_CANVAS_OFFSET,200,200));
    wgt->fillCanvas();
    wgt->repaint();
    engine = new Engine(wgt);
    wgt->bindEngine(engine);

    timeSpinBox = new QSpinBox(this);
    timeSpinBox->setGeometry(QRect(QPoint(170, 20), QSize(50, 30)));
    timeSpinBox->setRange(100, 2000);
    timeSpinBox->setSingleStep(100);
    timeSpinBox->setValue(400);
    timeSpinBox->setVisible(true);

    auto_button = new QPushButton("Auto", this);
    auto_button->setGeometry(QRect(QPoint(220, 20), QSize(40, 30)));

    help_button = new QPushButton("Help", this);
    help_button->setGeometry(QRect(QPoint(260, 20), QSize(40, 30)));

    save_button = new QPushButton("Save", this);
    save_button->setGeometry(QRect(QPoint(90, 20), QSize(40, 30)));

    new_button = new QPushButton("New", this);
    new_button->setGeometry(QRect(QPoint(10, 20), QSize(40, 30)));

    m_button = new QPushButton("Open", this);
    box_debug = new QCheckBox("Debug mode", this);
    box_debug->setGeometry(QRect(QPoint(10, 70), QSize(100, 20)));

    box_impacts = new QCheckBox("Show Impacts", this);
    box_impacts->setGeometry(QRect(QPoint(10, 50), QSize(100, 20)));

    x_offset_label = new QLabel("X Offset: ", this);
    x_offset_label->setGeometry(QRect(QPoint(10, 90), QSize(100, 20)));
    x_offset_label->setVisible(false);
    y_offset_label = new QLabel("Y Offset:", this);
    y_offset_label->setGeometry(QRect(QPoint(10, 110), QSize(100, 20)));
    y_offset_label->setVisible(false);

    x_canvas_size_label = new QLabel("X Canv. size:", this);
    x_canvas_size_label->setGeometry(QRect(QPoint(10, 130), QSize(140, 20)));
    x_canvas_size_label->setVisible(false);
    y_canvas_size_label = new QLabel("Y Canv. size:", this);
    y_canvas_size_label->setGeometry(QRect(QPoint(10, 150), QSize(140, 20)));
    y_canvas_size_label->setVisible(false);

    x_mouseover_label = new QLabel("X Mouseover: ", this);
    x_mouseover_label->setGeometry(QRect(QPoint(10, 170), QSize(140, 20)));
    x_mouseover_label->setVisible(false);
    y_mouseover_label = new QLabel("Y Mouseover:", this);
    y_mouseover_label->setGeometry(QRect(QPoint(10, 190), QSize(140, 20)));
    y_mouseover_label->setVisible(false);

    // устанавливаем размер и положение кнопки
    step_button = new QPushButton("Step", this);
    step_button->setGeometry(QRect(QPoint(130, 20),
    QSize(40, 30)));
    m_button->setGeometry(QRect(QPoint(50, 20),
    QSize(40, 30)));
    std::map<std::pair<int32_t,int32_t>, std::pair<float, bool>> empty_map;
    wgt->fillWithGex(10,1, empty_map, QPoint(engine->columns, engine->rows));

   // подключаем сигнал к соответствующему слоту
    connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));
    connect(step_button, SIGNAL (released()), this, SLOT (handleStepButton()));
    connect(auto_button, SIGNAL (released()), this, SLOT (handleAutoButton()));
    connect(box_debug, SIGNAL (stateChanged(int)), this, SLOT (handleDebugBox(int)));
    connect(box_impacts, SIGNAL (stateChanged(int)), this, SLOT (handleShowImpacts(int)));
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime())); // Подключаем сигнал таймера к нашему слоту
    connect(refresh_timer, SIGNAL(timeout()), this, SLOT(delayedRefresh()));
    connect(step_timer, SIGNAL(timeout()), this, SLOT(handleStepTimer()));
    connect(save_button, SIGNAL(released()), this, SLOT(handleSaveButton()));
    connect(new_button, SIGNAL(released()), this, SLOT(handleNewButton()));
    connect(help_button, SIGNAL(released()), this, SLOT(handleHelpButton()));
    this->setWindowTitle("Conway's game of Life");
}

void MainWindow::refreshDebugInfo()
{
    auto buf_point = wgt->getDisplayOffset();
    x_offset_label->setText("X Offset: " + QString::number(buf_point.x()) + " (" + QString::number(wgt->getDisplayOffset().x() % (wgt->halfHeight * 2)) + ")");
    y_offset_label->setText("Y Offset: " + QString::number(buf_point.y()) + " (" + QString::number(wgt->getDisplayOffset().y() % (wgt->halfWidth * 2 + wgt->getSizeOfGex())) + ")");
    buf_point = wgt->getCanvasSize();
    x_canvas_size_label->setText("X Canv. size: " + QString::number(wgt->size().width()) + " (" + QString::number(buf_point.x()) + ")");
    y_canvas_size_label->setText("Y Canv. size: " + QString::number(wgt->size().height()) + " (" + QString::number(buf_point.y()) + ")");
    auto buf_point2 = wgt->getDisplayOffset();
    Hex_number hex = wgt->getSelectedHexagon(QCursor::pos().x() - this->x() - wgt->x() - 8 + buf_point2.x(),
                                        QCursor::pos().y() - this->y() - wgt->y() - 31+ buf_point2.y());
    x_mouseover_label->setText("Curr. X Hex: "+ QString::number(hex.x) + " | " + QString::number(QCursor::pos().x() - this->x() - wgt->x() - 8 + buf_point2.x()));
    y_mouseover_label->setText("Curr. Y Hex: "+ QString::number(hex.y) + " | " + QString::number(QCursor::pos().y() - this->y() - wgt->y() - 31+ buf_point2.y()));
    ;
}

extern uint16_t offset;
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleShowImpacts(int toggle) {
    wgt->showImpacts = (bool)toggle;
}

void MainWindow::handleDebugBox(int toggle) {
    QMessageBox* msg = new QMessageBox(this->parentWidget());
    msg->setWindowTitle("Hello !");

    if(toggle)
    {
        x_offset_label->setVisible(true);
        y_offset_label->setVisible(true);
        x_canvas_size_label->setVisible(true);
        y_canvas_size_label->setVisible(true);
        x_mouseover_label->setVisible(true);
        y_mouseover_label->setVisible(true);
    }
    else
    {
        x_offset_label->setVisible(false);
        y_offset_label->setVisible(false);
        x_canvas_size_label->setVisible(false);
        y_canvas_size_label->setVisible(false);
        x_mouseover_label->setVisible(false);
        y_mouseover_label->setVisible(false);
    }
}

void MainWindow::delayedRefresh()
{
    wgt->resizeAndMove(QRect(X_CANVAS_OFFSET, Y_CANVAS_OFFSET ,this->width() - X_CANVAS_OFFSET, this->height() - Y_CANVAS_OFFSET));
    wgt->fillCanvas();
    wgt->fillWithGex(wgt->getSizeOfGex(), wgt->getThickness(), engine->getCells(), QPoint(engine->columns, engine->rows));
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   refresh_timer->start();
}

void MainWindow::handleHelpButton()
{
    HelpWindow *helpwindow = new HelpWindow(this->x() + this->width() / 2, this->y() + this->height() / 2);
    helpwindow->exec();
}

void MainWindow::handleButton()
{
    std::string filename;
    filename = QFileDialog::getOpenFileName(this,
         tr("Open Life File"), "\\", tr("Life Files (*.lf)")).toStdString();
    engine->loadFromFile(filename);
}

void MainWindow::handleStepButton()
{
    engine->step();
}

void MainWindow::updateTime()
{
    int i = 9;
    QMessageBox msgBox;
    this->repaint();
    refreshDebugInfo();
}

void MainWindow::handleStepTimer() {
    engine->step();
}

void MainWindow::handleSaveButton() {
    std::string filename;
    filename = QFileDialog::getSaveFileName(this,
                                            tr("Save to File"), "\\", tr("Life Files (*.lf)")).toStdString();
    engine->saveToFile(filename);
}

void MainWindow::callNewGame(int32_t xsize, int32_t ysize) {
    engine->startNewGame(xsize, ysize);
}

void MainWindow::handleNewButton()
{
//    QDialog newDialog( this );
//    newDialog.setModal( true );
//    newDialog.exec();
//    QMessageBox msgBox;
//    msgBox.setText("Samarin Romka, 14202");
//    msgBox.exec();
    NewGameWindow *newgamewindow = new NewGameWindow(this->x() + this->width() / 2, this->y() + this->height() / 2, this);
    newgamewindow->exec();
   // wgt->
}

void MainWindow::handleAutoButton() {
    if (step_timer->isActive()) {
        step_timer->stop();
        step_button->setEnabled(true);
        auto_button->setText("Auto");
    } else
    {
        step_timer->setInterval(timeSpinBox->value());
        step_timer->start();
        step_button->setEnabled(false);
        auto_button->setText("Stop");
    }
}
