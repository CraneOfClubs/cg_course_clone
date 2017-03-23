#include "newgamewindow.h"

NewGameWindow::NewGameWindow()
{

}

void NewGameWindow::handleButton() {
    _parent->callNewGame(xSpinBox->value(), ySpinBox->value());
    this->close();
}

NewGameWindow::NewGameWindow(int32_t xpos, int32_t ypos, MainWindow *parent)
{
    _parent = parent;
    this->setWindowTitle("New game");
    this->setGeometry(QRect(QPoint(xpos, ypos), QSize(200, 80)));

    save_button = new QPushButton("Start new game", this);
    save_button->setGeometry(QRect(QPoint(30, 60), QSize(110, 30)));

    xSpinBox = new QSpinBox(this);
    xSpinBox->setGeometry(QRect(QPoint(90, 20), QSize(50, 20)));
    xSpinBox->setRange(5, 100);
    xSpinBox->setSingleStep(1);
    xSpinBox->setValue(20);
    xSpinBox->setVisible(true);
    ySpinBox = new QSpinBox(this);
    ySpinBox->setGeometry(QRect(QPoint(90, 40), QSize(50, 20)));
    ySpinBox->setRange(5, 100);
    ySpinBox->setSingleStep(1);
    ySpinBox->setValue(20);
    ySpinBox->setVisible(true);

    xLabel = new QLabel("X Size: ", this);
    xLabel->setGeometry(QRect(QPoint(30, 20), QSize(100, 20)));
    yLabel = new QLabel("Y Size:", this);
    yLabel->setGeometry(QRect(QPoint(30, 40), QSize(100, 20)));
    setFixedSize(QSize(160, 120));
    connect(save_button, SIGNAL (released()), this, SLOT (handleButton()));
}

