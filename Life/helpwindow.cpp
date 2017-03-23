#include "helpwindow.h"

void HelpWindow::handleButton() {
    this->close();
}

HelpWindow::HelpWindow(int32_t xpos, int32_t ypos)
{
    this->setWindowTitle("Help");
    this->setGeometry(QRect(QPoint(xpos, ypos), QSize(190, 160)));

    quit_button = new QPushButton("Close", this);
    quit_button->setGeometry(QRect(QPoint(30, 120), QSize(130, 30)));
    mmbLabel = new QLabel("Hold MMB for scroll.", this);
    mmbLabel->setGeometry(QRect(QPoint(10, 20), QSize(200, 20)));
    rmbLabel = new QLabel("Hold/Press RMB for filling.", this);
    rmbLabel->setGeometry(QRect(QPoint(10, 40), QSize(200, 20)));
    lmbLabel = new QLabel("Hold/Press LMB for XORing.", this);
    lmbLabel->setGeometry(QRect(QPoint(10, 60), QSize(200, 20)));
    mbLabel = new QLabel("Made by Romka Samarin, 14202.", this);
    mbLabel->setGeometry(QRect(QPoint(10, 80), QSize(200, 20)));
    setFixedSize(QSize(190, 160));
    connect(quit_button, SIGNAL (released()), this, SLOT (handleButton()));
}
