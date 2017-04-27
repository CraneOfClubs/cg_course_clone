#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QMessageBox>

void MainWindow::slotNoImpl()
{
    // QMessageBox::information(0, "Message", "Not implemented");
}

void MainWindow::rotateFilter(QProgressBar *biba) {
    filtered_view->loadImage(filters->rotateImage(*(center_view->_previewImage), biba, settingsWidget->getFirstControl()));
}

void MainWindow::apply()
{
QProgressBar biba;
if (!center_view->image_loaded) {
    QMessageBox::information(0, "Message", "Select area first!");
    return;
}
    switch (current_filter) {
    case 0:
        QMessageBox::information(0, "Message", "Choose filter first!");
        break;
    case 1:
        filtered_view->loadImage(filters->rotateImage(*(center_view->_previewImage), &biba, settingsWidget->getFirstControl()));
        break;
    case 2:
        filtered_view->loadImage(filters->Watercolor(*(center_view->_previewImage), &biba, settingsWidget->getFirstControl()));
        break;
    case 3:
        filtered_view->loadImage(filters->orderedDithering(*(center_view->_previewImage), &biba, settingsWidget->getFirstControl()));
        break;
    case 4:
        filtered_view->loadImage(filters->FloydSteinbergFilter(*(center_view->_previewImage), &biba, settingsWidget->getFirstControl(), settingsWidget->getSecondControl(), settingsWidget->getThirdControl(),4));
        break;
    case 5:
        filtered_view->loadImage(filters->grayScale(*(center_view->_previewImage), &biba));
        break;
    case 6:
        filtered_view->loadImage(filters->Embossing(*(center_view->_previewImage), &biba));
        break;
    case 7:
        filtered_view->loadImage(filters->Scale(*(center_view->_previewImage), &biba, settingsWidget->getFirstControl()));
        break;
    case 8:
        filtered_view->loadImage(filters->Negative(*(center_view->_previewImage), &biba));
        break;
    case 9:
        filtered_view->loadImage(filters->Edge(*(center_view->_previewImage), &biba, settingsWidget->getFirstControl()));
        break;
    case 10:
        filtered_view->loadImage(filters->Roberts(*(center_view->_previewImage), &biba, settingsWidget->getFirstControl()));
        break;
    case 11:
        filtered_view->loadImage(filters->Sobel(*(center_view->_previewImage), &biba, settingsWidget->getFirstControl()));
        break;
    case 13:
        filtered_view->loadImage(filters->SimpleBlur(*(center_view->_previewImage), &biba));
        break;
    case 14:
        filtered_view->loadImage(filters->GaussianBlur(*(center_view->_previewImage), &biba));
        break;
    case 15:
        filtered_view->loadImage(filters->GaussianBlur(*(center_view->_previewImage), &biba));
        break;
    case 16:
        filtered_view->loadImage(filters->GammaCorrection(*(center_view->_previewImage), &biba, settingsWidget->getFirstControl()));
        break;
    case 17:
        filtered_view->loadImage(filters->Sharpness(*(center_view->_previewImage), &biba));
        break;
    }
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
}

void MainWindow::Watercolor() {
    settingsWidget->setFirstControl("Radius", 1, 10, 1, 0, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 2;
}

void MainWindow::OrderedDither() {
    settingsWidget->setFirstControl("Matrix size", 1, 10, 1, 0, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 3;
}

void MainWindow::Floyd() {
    settingsWidget->setFirstControl("Red", 2.0, 255.0, 1, 0, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, true, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, true, false);
    current_filter = 4;
}

void MainWindow::Grayscale() {
    settingsWidget->setFirstControl("Red", 2.0, 255.0, 1, 0, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 5;
}

void MainWindow::Embossing() {
    settingsWidget->setFirstControl("Red", 2.0, 255.0, 1, 0, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 6;
}

void MainWindow::Scale() {
    settingsWidget->setFirstControl("Scale factor", -5, 5, 1, 0, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 7;
}

void MainWindow::Negative() {
    settingsWidget->setFirstControl("Red", 2.0, 255.0, 1, 0, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 8;
}

void MainWindow::Edge() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 9;
}

void MainWindow::Roberts() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 10;
}

void MainWindow::Sobel() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, true, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 11;
}

void MainWindow::Blur() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 12;
}

void MainWindow::SimpleBlur() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 13;
}

void MainWindow::GaussianBlur() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 14;
}

void MainWindow::BoxBlur() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 15;
}

void MainWindow::Gamma() {
    settingsWidget->setFirstControl("Value", 0.01, 12, 1, 1, true, true);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 16;
}

void MainWindow::Sharpness() {
    settingsWidget->setFirstControl("Threshold", 0, 500, 1, 1, false, false);
    settingsWidget->setSecondControl("Green", 2.0, 255.0, 1, 1, false, false);
    settingsWidget->setThirdControl("Blue", 2.0, 255.0, 1, 2, false, false);
    current_filter = 17;
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
    ptb->addAction(QIcon(":/img/apply.png"), tr("Apply"), this, SLOT(apply()));

    return ptb;
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
