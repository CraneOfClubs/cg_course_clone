#include "helpwindow.h"

void HelpWindow::handleButton() {
    this->close();
}

HelpWindow::HelpWindow(int32_t xpos, int32_t ypos)
{
    this->setWindowTitle("Help");
    this->setGeometry(QRect(QPoint(xpos, ypos), QSize(250, 250)));

    quit_button = new QPushButton("Close", this);
    quit_button->setGeometry(QRect(QPoint(80, 150), QSize(130, 30)));
    rmbLabel = new QLabel("Press LMB for putting user isoline (at canvas)", this);
    rmbLabel->setGeometry(QRect(QPoint(10, 40), QSize(250, 20)));
    lmbLabel = new QLabel("Press RMB for removing user isoline (everywhere)", this);
    lmbLabel->setGeometry(QRect(QPoint(10, 60), QSize(250, 20)));
    mbLabel = new QLabel("Made by Romka Samarin, 14202.", this);
    mbLabel->setGeometry(QRect(QPoint(10, 80), QSize(200, 20)));
    setFixedSize(QSize(300, 190));
    connect(quit_button, SIGNAL (released()), this, SLOT (handleButton()));
}
