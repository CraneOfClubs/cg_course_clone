#include "settingswindow.h"

SettingsWindow::SettingsWindow()
{

}

void SettingsWindow::handleFirstImpact(int val) {
    firstImpactLabel->setText("First Impact: " + QString::number(((double)firstImpact->value()) / 10));
}

void SettingsWindow::handleSecondImpact(int val) {
    secondImpactLabel->setText("Second Impact: " + QString::number(((double)secondImpact->value()) / 10));
}

void SettingsWindow::handleLiveBegin(int val) {
    liveBeginLabel->setText("Live Begin: " + QString::number(((double)liveBegin->value()) / 10));
}

void SettingsWindow::handleLiveEnd(int val) {
    liveEndLabel->setText("Live End: " + QString::number(((double)liveEnd->value()) / 10));
}

void SettingsWindow::handleBirthEnd(int val) {
    birthEndLabel->setText("Birth End: " + QString::number(((double)birthEnd->value()) / 10));
}

void SettingsWindow::handleBirthBegin(int val) {
    birthBeginLabel->setText("Birth Begin: " + QString::number(((double)birthBegin->value()) / 10));
}

void SettingsWindow::handleButton() {
    _parent->engine->setSettings(((double)firstImpact->value()) / 10, ((double)secondImpact->value()) / 10, ((double)liveBegin->value()) / 10, ((double)liveEnd->value()) / 10,
                                 ((double)birthBegin->value()) / 10, ((double)birthEnd->value()) / 10);
    this->close();
}

SettingsWindow::SettingsWindow(int32_t xpos, int32_t ypos, MainWindow *parent)
{
    _parent = parent;
    this->setWindowTitle("Life rules");
    this->setGeometry(QRect(QPoint(xpos, ypos), QSize(200, 80)));



    save_button = new QPushButton("Accept", this);
    save_button->setGeometry(QRect(QPoint(30, 200), QSize(110, 30)));

    firstImpact = new QSlider(Qt::Horizontal, this);
    firstImpact->setGeometry(QRect(QPoint(10, 20), QSize(80, 20)));
    firstImpact->setTickInterval(1);
    firstImpact->setMinimum(0);
    firstImpact->setMaximum(50);
    firstImpact->setSingleStep(1);
    firstImpact->setValue(round(parent->engine->_settings.firstImpact * 10));

    firstImpactLabel = new QLabel("First Impact: " + QString::number(parent->engine->_settings.firstImpact), this);
    firstImpactLabel->setGeometry(QRect(QPoint(100, 20), QSize(100, 20)));

    secondImpact = new QSlider(Qt::Horizontal, this);
    secondImpact->setGeometry(QRect(QPoint(10, 50), QSize(80, 20)));
    secondImpact->setTickInterval(1);
    secondImpact->setMinimum(0);
    secondImpact->setMaximum(50);
    secondImpact->setSingleStep(1);
    secondImpact->setValue(round(parent->engine->_settings.secondImpact * 10));

    secondImpactLabel = new QLabel("Second Impact: " + QString::number(parent->engine->_settings.secondImpact), this);
    secondImpactLabel->setGeometry(QRect(QPoint(100, 50), QSize(100, 20)));

    liveBegin = new QSlider(Qt::Horizontal, this);
    liveBegin->setGeometry(QRect(QPoint(10, 80), QSize(80, 20)));
    liveBegin->setTickInterval(1);
    liveBegin->setMinimum(0);
    liveBegin->setMaximum(50);
    liveBegin->setSingleStep(1);
    liveBegin->setValue(round(parent->engine->_settings.liveBegin * 10));

    liveBeginLabel = new QLabel("Live Begin: " + QString::number(parent->engine->_settings.liveBegin), this);
    liveBeginLabel->setGeometry(QRect(QPoint(100, 80), QSize(100, 20)));

    liveEnd = new QSlider(Qt::Horizontal, this);
    liveEnd->setGeometry(QRect(QPoint(10, 110), QSize(80, 20)));
    liveEnd->setTickInterval(1);
    liveEnd->setMinimum(0);
    liveEnd->setMaximum(50);
    liveEnd->setSingleStep(1);
    liveEnd->setValue(round(parent->engine->_settings.liveEnd * 10));

    liveEndLabel = new QLabel("Live End: " + QString::number(parent->engine->_settings.liveEnd), this);
    liveEndLabel->setGeometry(QRect(QPoint(100, 110), QSize(100, 20)));

    birthBegin = new QSlider(Qt::Horizontal, this);
    birthBegin->setGeometry(QRect(QPoint(10, 140), QSize(80, 20)));
    birthBegin->setTickInterval(1);
    birthBegin->setMinimum(0);
    birthBegin->setMaximum(50);
    birthBegin->setSingleStep(1);
    birthBegin->setValue(round(parent->engine->_settings.birthBegin * 10));

    birthBeginLabel = new QLabel("Birth Begin: " + QString::number(parent->engine->_settings.birthBegin), this);
    birthBeginLabel->setGeometry(QRect(QPoint(100, 140), QSize(100, 20)));

    birthEnd = new QSlider(Qt::Horizontal, this);
    birthEnd->setGeometry(QRect(QPoint(10, 170), QSize(80, 20)));
    birthEnd->setTickInterval(1);
    birthEnd->setMinimum(0);
    birthEnd->setMaximum(50);
    birthEnd->setSingleStep(1);
    birthEnd->setValue(round(parent->engine->_settings.birthEnd * 10));

    birthEndLabel = new QLabel("Birth End: " + QString::number(parent->engine->_settings.birthEnd), this);
    birthEndLabel->setGeometry(QRect(QPoint(100, 170), QSize(100, 20)));

    setFixedSize(QSize(220, 260));
    connect(save_button, SIGNAL (released()), this, SLOT (handleButton()));
    connect(firstImpact, SIGNAL(valueChanged(int)), this, SLOT(handleFirstImpact(int)));
    connect(secondImpact, SIGNAL(valueChanged(int)), this, SLOT(handleSecondImpact(int)));
    connect(liveBegin, SIGNAL(valueChanged(int)), this, SLOT(handleLiveBegin(int)));
    connect(liveEnd, SIGNAL(valueChanged(int)), this, SLOT(handleLiveEnd(int)));
    connect(birthBegin, SIGNAL(valueChanged(int)), this, SLOT(handleBirthBegin(int)));
    connect(birthEnd, SIGNAL(valueChanged(int)), this, SLOT(handleBirthEnd(int)));
}
