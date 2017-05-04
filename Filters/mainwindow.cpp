#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QMessageBox>

void MainWindow::slotNoImpl()
{
    // QMessageBox::information(0, "Message", "Not implemented");
}

void MainWindow::rotateFilter(QProgressBar *biba) {
    filtered_view->loadImage(filters->rotateImage(*(center_view->_previewImage), settingsWidget->getFirstControl()));
}

void MainWindow::loadFiltered(QImage image) {
    filtered_view->loadImage(image);
    disableEditing(false);
}

void MainWindow::apply()
{
QProgressBar biba;
if (!center_view->image_loaded) {
    QMessageBox::information(0, "Message", "Select area first!");
    return;
}
    disableEditing(true);
    f_handler->wait();
    f_handler->loadImage(*(center_view->_previewImage), current_filter, settingsWidget->getFirstControl(), settingsWidget->getSecondControl(), settingsWidget->getThirdControl());
    f_handler->start();
}


void MainWindow::openImage() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open image"), QDir::currentPath(), tr("Images (*.png *.bmp)"));
    if (filename.length() == 0) {
        showError(tr("Please choose file to open!"));
        return;
    }
    QImageReader reader(filename);
    if (!reader.canRead()) {
        showError(tr("Cannot open file %1 to load image").arg(filename));
        return;
    }
    QImage image = reader.read();
    if (image.isNull()) {
        showError(tr("Cannot decode image!"));
        return;
    }
    if (image.format() != QImage::Format_RGBA8888) {
        image = image.convertToFormat(QImage::Format_RGBA8888);
    }
    sourceImage->loadImage(image);
}

void MainWindow::Rotate() {
    settingsWidget->setFirstControl("Angle", -180, 180, 1, 0, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 1;
    apply();
}

void MainWindow::Watercolor() {
    settingsWidget->setFirstControl("Radius", 1, 10, 1, 0, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 2;
    apply();
}

void MainWindow::OrderedDither() {
    settingsWidget->setFirstControl("Matrix size", 1, 10, 1, 0, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 3;
    apply();
}

void MainWindow::Floyd() {
    settingsWidget->setFirstControl("Red", 2.0, 255.0, 1, 0, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, true, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, true, false);
    current_filter = 4;
    apply();
}

void MainWindow::Grayscale() {
    settingsWidget->setFirstControl("Red", 2.0, 255.0, 1, 0, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 5;
    apply();
}

void MainWindow::Embossing() {
    settingsWidget->setFirstControl("Red", 2.0, 255.0, 1, 0, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 6;
    apply();
}

void MainWindow::Scale() {
    settingsWidget->setFirstControl("Scale factor", -5, 5, 1, 0, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 7;
    apply();
}

void MainWindow::Negative() {
    settingsWidget->setFirstControl("Red", 2.0, 255.0, 1, 0, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 8;
    apply();
}

void MainWindow::Edge() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 9;
    apply();
}

void MainWindow::Roberts() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 10;
    apply();
}

void MainWindow::Sobel() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 11;
    apply();
}

void MainWindow::Blur() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 12;
    apply();
}

void MainWindow::SimpleBlur() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 13;
    apply();
}

void MainWindow::GaussianBlur() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 14;
    apply();
}

void MainWindow::BoxBlur() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 15;
    apply();
}

void MainWindow::Gamma() {
    settingsWidget->setFirstControl("Value", 0.01, 12, 1, 1, true, true);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 16;
    apply();
}

void MainWindow::Sharpness() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 17;
    apply();
}

void MainWindow::LoadBack() {
    center_view->loadImage(*(filtered_view->_previewImage));
}

void MainWindow::LoadRight() {
    filtered_view->loadImage(*(center_view->_previewImage));
}

void MainWindow::saveImage() {
    if (!center_view->image_loaded) {
        QMessageBox::information(0, "Message", "Select area first!");
        return;
    }
    std::string filename;
    filename = QFileDialog::getSaveFileName(this, tr("Save to File"), "\\", tr("Image (*.png)")).toStdString();
    center_view->_previewImage->save(QString::fromStdString(filename), "PNG");
}


QToolBar* MainWindow::createToolBar()
{
    QToolBar* ptb = new QToolBar("Main Toolbar");

    ptb->addAction(QIcon(":/img/buttons/img/open.png"), tr("Open File"), this, SLOT(openImage()));
    ptb->addAction(QIcon(":/img/buttons/img/save.jpg"), tr("Save File"), this, SLOT(saveImage()));
    ptb->addSeparator();
    ptb->addAction(QIcon(":/img/rotate.png"), tr("Rotate"), this, SLOT(Rotate()));
    ptb->addAction(QIcon(":/img/dither.png"), tr("Floyd dither"), this, SLOT(Floyd()));
    ptb->addAction(QIcon(":/img/dither.png"), tr("Ordered dither"), this, SLOT(OrderedDither()));
    ptb->addAction(QIcon(":/img/grayscale.png"), tr("Grayscale"), this, SLOT(Grayscale()));
    ptb->addAction(QIcon(":/img/emboss.jpg"), tr("Embossing"), this, SLOT(Embossing()));
    ptb->addAction(QIcon(":/img/zoom.png"), tr("Scale"), this, SLOT(Scale()));
    ptb->addAction(QIcon(":/img/negative.jpg"), tr("Negative"), this, SLOT(Negative()));
    ptb->addAction(QIcon(":/img/edge.png"), tr("Edge"), this, SLOT(Edge()));
    ptb->addAction(QIcon(":/img/roberts.png"), tr("Roberts"), this, SLOT(Roberts()));
    ptb->addAction(QIcon(":/img/sobel.png"), tr("Sobel"), this, SLOT(Sobel()));
    ptb->addAction(QIcon(":/img/blur.png"), tr("Blur"), this, SLOT(SimpleBlur()));
    ptb->addAction(QIcon(":/img/gaussian_blur.png"), tr("Gaussian Blur"), this, SLOT(GaussianBlur()));
    ptb->addAction(QIcon(":/img/box_blur.png"), tr("Box Blur"), this, SLOT(BoxBlur()));
    ptb->addAction(QIcon(":/img/gamma.png"), tr("Gamma Correction"), this, SLOT(Gamma()));
    ptb->addAction(QIcon(":/img/sharpness.png"), tr("Sharpness"), this, SLOT(Sharpness()));
    ptb->addAction(QIcon(":/img/watercolor.png"), tr("Watercolor"), this, SLOT(Watercolor()));
    ptb->addSeparator();
    ptb->addAction(QIcon(":/img/arrow_back.jpg"), tr("Load Back"), this, SLOT(LoadBack()));
    ptb->addAction(QIcon(":/img/arrow_front.jpg"), tr("Load to Filtered"), this, SLOT(LoadRight()));
    ptb->addSeparator();
    ptb->addAction(QIcon(":/img/apply.png"), tr("Apply"), this, SLOT(apply()));

    return ptb;
}

void MainWindow::disableEditing(bool res)
{
    settingsWidget->firstControlSlider->setEnabled(!res);
    settingsWidget->secondControlSlider->setEnabled(!res);
    settingsWidget->thirdControlSlider->setEnabled(!res);

    settingsWidget->firstControlSpinBox->setEnabled(!res);
    settingsWidget->secondControlSpinBox->setEnabled(!res);
    settingsWidget->thirdControlSpinBox->setEnabled(!res);
}

void MainWindow::showError(QString text) {
    QErrorMessage error_message;
    error_message.setModal(true);
    error_message.showMessage(text);
    error_message.exec();
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    progress = new QProgressBar(this);
    progress->setMinimum(0);
    progress->setMaximum(100);
    progress->setGeometry(20, 500, 500, 30);

    f_handler = new FilterHandler(this, progress->maximum());
    connect(f_handler, SIGNAL(valueChanged(int)), progress, SLOT(setValue(int)));

    connect(f_handler, SIGNAL(imageReady(QImage)), this, SLOT(loadFiltered(QImage)));

    QPushButton *pbRun = new QPushButton("Run", this);
    pbRun->setGeometry(500,500,500,500);
    connect(pbRun, SIGNAL(clicked()), f_handler, SLOT(start()));

    filters = new FiltersCluster;
    settingsWidget = new SettingWidget(this, 500, 200);
    setMouseTracking(true);
    addToolBar(Qt::TopToolBarArea, createToolBar());
    sourceImage = new Canvas(this, 300, 300, this);
    center_view = new PicView(this, 302, 302);
    filtered_view = new FilteredView(this, 302, 302);
    this->setGeometry(400,400, 1200, 600);

}

void MainWindow::selectHandler(QImage image_to_place) {
    center_view->loadImage(image_to_place);
    filtered_view->loadImage(image_to_place);
}


MainWindow::~MainWindow()
{
    delete ui;
}
