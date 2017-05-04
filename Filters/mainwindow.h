#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QToolBar>
#include <QErrorMessage>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QString>
#include <QProgressBar>
#include <canvas.h>
#include <picview.h>
#include <filteredview.h>
#include "settingwidget.h"
#include <filterscluster.h>
#include "filterhandler.h"

namespace Ui {
class MainWindow;
}

class Canvas;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showError(QString text);
    void selectHandler(QImage image_to_place);
private:
    FilterHandler *f_handler;
    Ui::MainWindow *ui;
    QToolBar *createToolBar();
    QProgressBar* progress;
    Canvas* sourceImage;
    QGroupBox* horizontalGroupBox;
    SettingWidget* settingsWidget;
    PicView* center_view;
    FilteredView* filtered_view;
    FiltersCluster* filters;
    uint8_t current_filter = 0;
    void setSettingsControls();
    void disableEditing(bool res);
    void setFirstControl(QString name, double start_pos, double end_pos, double step_amount, int position, bool visible, bool fp);
    void rotateFilter(QProgressBar *biba);
protected:
   // void mouseMoveEvent(QMouseEvent* event);
   // void mouseReleaseEvent(QMouseEvent *event);
public slots:
    void LoadBack();
    void saveImage();
    void Embossing();
    void slotNoImpl();
    void apply();
    void openImage();
    void Rotate();
    void Floyd();
    void OrderedDither();
    void Grayscale();
    void Scale();
    void Negative();
    void Edge();
    void Roberts();
    void Sobel();
    void Blur();
    void GaussianBlur();
    void BoxBlur();
    void SimpleBlur();
    void Gamma();
    void Sharpness();
    void Watercolor();
    void LoadRight();
    void loadFiltered(QImage image);
};

#endif // MAINWINDOW_H
