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

    settings_button = new QPushButton("Settings", this);
    settings_button->setGeometry(QRect(QPoint(300, 20), QSize(60, 30)));
    //settings_button->setVisible(false);

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
    std::map<std::pair<int32_t,int32_t>, std::pair<double, bool>> empty_map;
    wgt->fillWithGex(10,1, empty_map, QPoint(engine->columns, engine->rows));

    vertScroll = new QScrollBar(Qt::Vertical, this);
    vertScroll->setGeometry(QRect(QPoint(130 + wgt->width(), 50), QSize(20, wgt->height())));
    horisScroll = new QScrollBar(Qt::Horizontal, this);
    horisScroll->setGeometry(QRect(QPoint(130, 280), QSize(wgt->width(), 20)));
    vertScroll->setSingleStep(1);
    horisScroll->setSingleStep(1);
    vertScroll->setMinimum(0);
    horisScroll->setMinimum(0);

    fileMenu = menuBar()->addMenu(tr("File"));
    gameMenu = menuBar()->addMenu(tr("Game"));
    helpMenu = menuBar()->addMenu(tr("?"));

    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL (triggered()), this, SLOT(handleNewButton()));

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open saved file"));
    connect(openAct, SIGNAL (triggered()), this, SLOT(handleButton()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save to file"));
    connect(saveAct, SIGNAL (triggered()), this, SLOT(handleSaveButton()));

    exitAct = new QAction(tr("&Quit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Quit application"));
    connect(exitAct, SIGNAL (triggered()), this, SLOT(handleQuit()));

    stepAct = new QAction(tr("&Step"), this);
    stepAct->setShortcuts(QKeySequence::Forward);
    stepAct->setStatusTip(tr("Do step"));
    connect(stepAct, SIGNAL (triggered()), this, SLOT(handleStepButton()));

    autoAct = new QAction(tr("&Auto"), this);
    autoAct->setShortcuts(QKeySequence::Paste);
    autoAct->setStatusTip(tr("Engage autostepping"));
    connect(autoAct, SIGNAL (triggered()), this, SLOT(handleAutoButton()));

    settingsAct = new QAction(tr("&Settings"), this);
    settingsAct->setShortcuts(QKeySequence::Preferences);
    settingsAct->setStatusTip(tr("Open settings window"));
    connect(settingsAct, SIGNAL (triggered()), this, SLOT(handleSettingsButton()));

    impactsAct = new QAction(tr("&Show impacts"), this);
    impactsAct->setShortcuts(QKeySequence::Refresh);
    impactsAct->setStatusTip(tr("Toggle show impacts off/on"));
    connect(impactsAct, SIGNAL (triggered()), this, SLOT(toggleImpacts()));

    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    gameMenu->addAction(stepAct);
    gameMenu->addAction(autoAct);
    gameMenu->addSeparator();
    gameMenu->addAction(impactsAct);
    gameMenu->addAction(settingsAct);

    helpAct = new QAction(tr("&Help"), this);
    helpAct->setShortcuts(QKeySequence::HelpContents);
    helpAct->setStatusTip(tr("Call help window"));
    connect(helpAct, SIGNAL (triggered()), this, SLOT(handleHelpButton()));

    helpMenu->addAction(helpAct);

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
    connect(vertScroll, SIGNAL(valueChanged(int)), this, SLOT(handleVertScroll()));
    connect(settings_button, SIGNAL(released()), this, SLOT(handleSettingsButton()));
    this->setWindowTitle("Conway's game of Life");
    delayedRefresh();
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

void MainWindow::toggleImpacts() {
    wgt->showImpacts ^= true;
    box_impacts->setChecked(wgt->showImpacts);
    if (wgt->showImpacts) {
        impactsAct->setText(tr("Hide impacts"));
    } else
        impactsAct->setText(tr("Show impacts"));
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
    wgt->resizeAndMove(QRect(X_CANVAS_OFFSET, Y_CANVAS_OFFSET ,this->width() - X_CANVAS_OFFSET - 20, this->height() - Y_CANVAS_OFFSET - 20));
    vertScroll->setGeometry(QRect(QPoint(130 + wgt->width(), 50), QSize(20, wgt->height())));
    horisScroll->setGeometry(QRect(QPoint(130, 50 + wgt->height()), QSize(wgt->width(), 20)));
    int32_t drawing_x = wgt->halfWidth * 2 * engine->columns + wgt->halfWidth;
    int32_t drawing_y = (wgt->halfHeight + wgt->getSizeOfGex()) * engine->rows + wgt->halfHeight;

    horisScroll->setMaximum(abs(wgt->width() - drawing_x));
    vertScroll->setMaximum(abs(wgt->height() - drawing_y));

    if (drawing_x < wgt->width())
        horisScroll->setEnabled(false);
    else
        horisScroll->setEnabled(true);

    if (drawing_y < wgt->height())
        vertScroll->setEnabled(false);
    else
        vertScroll->setEnabled(true);

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

void MainWindow::handleSettingsButton()
{
    SettingsWindow *settingswindow = new SettingsWindow(this->x() + this->width() / 2, this->y() + this->height() / 2, this);
    settingswindow->exec();
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

void MainWindow::handleQuit()
{
    this->close();
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

void MainWindow::handleVertScroll()
{
    wgt->setDisplayOffset(-1, vertScroll->value());
//        QDialog newDialog( this );
//        newDialog.setModal( true );
//        newDialog.exec();
//        QMessageBox msgBox;
//        msgBox.setText("Samarin Romka, 14202");
//        msgBox.exec();
}

void MainWindow::handleHorisScroll()
{
    wgt->setDisplayOffset(horisScroll->value(), -1);
}

void MainWindow::handleAutoButton() {
    if (step_timer->isActive()) {
        step_timer->stop();
        step_button->setEnabled(true);
        stepAct->setEnabled(true);
        auto_button->setText("Auto");
        autoAct->setText("Auto");
    } else
    {
        step_timer->setInterval(timeSpinBox->value());
        step_timer->start();
        step_button->setEnabled(false);
        stepAct->setEnabled(false);
        auto_button->setText("Stop");
        autoAct->setText("Stop");
    }
}
