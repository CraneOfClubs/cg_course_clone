#ifndef HELPWINDOW_H
#define HELPWINDOW_H
#include <QDialog>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class HelpWindow;
}

class HelpWindow : public QDialog
{
    Q_OBJECT

public:
    HelpWindow();
    HelpWindow(int32_t xpos, int32_t ypos);
private:
    QLabel* mmbLabel;
    QLabel* rmbLabel;
    QLabel* lmbLabel;
    QLabel* mbLabel;
    QPushButton* quit_button;
private slots:
      void handleButton();
};

#endif // HELPWINDOW_H
