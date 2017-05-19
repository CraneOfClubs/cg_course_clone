#include "canvas.h"
#include <QMouseEvent>

void Canvas::loadImage(QImage image)
{
    *_image = image;
    this->repaint();
}


Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    _parent = parent;
    _image = new QImage(QSize(this->width(), this->height()), QImage::Format_RGBA8888);
    memset(_image->bits(), 255, _image->height() * _image->width() * _image->depth() / 8);
    setMouseTracking(true);
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
    MainWindow *mw = dynamic_cast<MainWindow*>(_parent);
    double val = mw->getEngine()->getValueForPixel(this->width(), this->height(), event->x(), event->y());
    mw->legend->setCurrentValue(val);
}


void Canvas::drawLineBresenham(QPoint &from, QPoint &to, uint32_t color) {
     if (abs(to.y() - from.y()) >= abs(to.x() - from.x())) {
         if (from.y() > to.y()) {
             std::swap(from, to);
         }
         uint32_t x = from.x();
         int32_t y;
         uint32_t dx = abs(to.x() - from.x());
         uint32_t dy = to.y() - from.y();
         int32_t offset = 2 * dx - dy;
         for (y = from.y(); y <= to.y(); ++y) {
             storePixel(_image, x, y, color);
             if (offset > 0) {
                 x += to.x() > from.x() ? 1 : -1;
                 offset = offset - 2 * dy;
             }
             offset = offset + 2 * dx;
         }
         return;
     }
     if (from.x() > to.x()) {
         std::swap(from, to);
     }
     uint32_t y = from.y();
     int32_t x;
     uint32_t dx = to.x() - from.x();
     uint32_t dy = abs(to.y() - from.y());
     int32_t offset = 2 * dy - dx;
     for (x = from.x(); x <= to.x(); ++x) {
         storePixel(_image, x, y, color);
         if (offset > 0) {
             y += to.y() > from.y() ? 1 : -1;
             offset = offset - 2 * dx;
         }
         offset = offset + 2 * dy;
     }
 }



void Canvas::storePixel(QImage *image, int32_t x, int32_t y, uint32_t color){
    if (x < 0 || y < 0 || x >= image->width() || y >= image->height())
        return;

    uint32_t offset = x *image->depth()/8 + y * image->bytesPerLine();
    image->bits()[offset] = color >> 24 & 0xFF;
    image->bits()[offset + 1] = color >> 16 & 0xFF;
    image->bits()[offset + 2] = color >> 8 & 0xFF;
    image->bits()[offset + 3] = color & 0xFF;
}

void Canvas::redraw() {
    MainWindow *mw = dynamic_cast<MainWindow*>(_parent);
    loadImage(mw->getEngine()->generateImage(this->width(), this->height(),  mw->_interpolate));
    if (mw->_drawisolines)
        drawIsolines(mw->grid_rows, mw->grid_cols);
    if (mw->_drawgrid)
        drawMesh(mw->grid_rows, mw->grid_cols);
    mw->legend->recalcImage();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    MainWindow *mw = dynamic_cast<MainWindow*>(_parent);
    if (event->button() == Qt::LeftButton)
        mw->getEngine()->setUserIsoline(mw->getEngine()->getValueForPixel(this->width(), this->height(), event->x(), event->y()), true);
    if (event->button() == Qt::RightButton)
        mw->getEngine()->setUserIsoline(0.0, false);
    this->redraw();
}

void Canvas::drawIsolines(int32_t rows, int32_t cols){
    double cell_width = (double)(_image->width()) / (double)(cols);
    double cell_height = (double)(_image->height()) / (double)(rows);
    MainWindow *mw = dynamic_cast<MainWindow*>(_parent);
    auto lims = mw->getEngine()->getFuncLimits();

    double iter_x = (lims.second.first - lims.first.first) / (double)_image->width();
    double iter_y = (lims.second.second - lims.first.second) / (double)_image->height();

    for (int32_t j = 0; j < cols; ++j) {
        for (int32_t i = 0; i < rows; ++i) {
            int32_t x = round(i * cell_width);
            int32_t x_next = round((i + 1) * cell_width);
            int32_t y = round(j * cell_height);
            int32_t y_next = round((j + 1) * cell_height);
            double x_f = i * iter_x * cell_width + lims.first.first;
            double x_next_f = (i + 1) * iter_x * cell_width + lims.first.first;
            double y_f = j * iter_y * cell_height + lims.first.second;
            double y_next_f = (j + 1) * iter_y * cell_height + lims.first.second;
            std::vector<std::pair<QPoint, double>> cell
            {
                std::make_pair(QPoint(x, y), mw->getEngine()->function(x_f, y_f)),
                std::make_pair(QPoint(x_next, y), mw->getEngine()->function(x_next_f, y_f)),
                std::make_pair(QPoint(x, y_next), mw->getEngine()->function(x_f, y_next_f)),
                std::make_pair(QPoint(x_next, y_next), mw->getEngine()->function(x_next_f, y_next_f))
            };
            double median = mw->getEngine()->function(i * cell_width / 2 + lims.first.first, (j * cell_height / 2) + lims.first.second);
            auto levels = mw->getEngine()->getAllLevels();
            std::vector<std::pair<QPoint, QPoint>> lines;
            for (int32_t k = 0; k < mw->getEngine()->getAmountOfLevels(); ++k) {
                int32_t points_bigger = 0;
                for (auto it = cell.begin(); it < cell.end(); ++it) {
                           if (it->second > levels[k])
                               ++points_bigger;
                       }
                       if (points_bigger == 1) {
                           lines = mw->getEngine()->handleSingleCell(cell, levels[k]);
                       }                       
                       if (points_bigger == 2) {
                          lines = mw->getEngine()->calcComplicatedMesh(cell, levels[k], median);
                       }
                       if (points_bigger == 3) {
                           lines = mw->getEngine()->handleTripleCell(cell, levels[k]);
                       }
                for (auto it = lines.begin(); it < lines.end(); ++it) {
                    drawLineBresenham(it->first, it->second, 0x000000FF);
                }
            }
            if (mw->getEngine()->isUserIsolineActive()) {
                int32_t points_bigger = 0;
                for (auto it = cell.begin(); it < cell.end(); ++it) {
                           if (it->second > mw->getEngine()->getUserIsolineLevel())
                               ++points_bigger;
                       }
                       if (points_bigger == 1) {
                           lines = mw->getEngine()->handleSingleCell(cell, mw->getEngine()->getUserIsolineLevel());
                       }
                       if (points_bigger == 2) {
                          lines = mw->getEngine()->calcComplicatedMesh(cell, mw->getEngine()->getUserIsolineLevel(), median);
                       }
                       if (points_bigger == 3) {
                           lines = mw->getEngine()->handleTripleCell(cell, mw->getEngine()->getUserIsolineLevel());
                       }
                for (auto it = lines.begin(); it < lines.end(); ++it) {
                    drawLineBresenham(it->first, it->second, 0x000000FF);
                }
            }
        }
    }
    this->repaint();
}

void Canvas::drawMesh(int32_t rows, int32_t cols)
{
    double x_pos = 0;
    double y_pos = 0;
    for (int32_t x = 0; x < cols; ++x) {
        x_pos += (double)(_image->width()) / (double)(cols);
        for (int32_t i = 0; i < _image->height(); ++i)
        storePixel(_image, round(x_pos), i, 0x000000FF);
    }

    for (int32_t y = 0; y < rows; ++y) {
        y_pos += (double)(_image->height()) / (double)(rows);
        for (int32_t i = 0; i < _image->width(); ++i)
        storePixel(_image, i, round(y_pos), 0x000000FF);
    }
    this->repaint();
}

void Canvas::resizeEvent(QResizeEvent *e)
{
    MainWindow *mw = dynamic_cast<MainWindow*>(_parent);
    loadImage(mw->getEngine()->generateImage(this->width(), this->height(), mw->_interpolate));
    this->redraw();
    repaint();
}


void Canvas::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.drawImage(QRect(0, 0, _image->size().width(), _image->size().height()), *_image);
}
