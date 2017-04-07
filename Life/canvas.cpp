#include "canvas.h"
#include "QMessageBox"

//void Canvas::fillGexes(std::map<std::pair<int32_t, int32_t>, bool> cells);

Canvas::Canvas(QWidget * parent, int32_t size_x, int32_t size_y) : QWidget(parent)
{
    this->size_x = size_x;
    this->size_y = size_y;
    image = new QImage(QSize(size_x,size_y), QImage::Format_RGB32);
    setMouseTracking(true);
}

void Canvas::addScrolls(QScrollBar* horis, QScrollBar* vert) {
    horisScroll = horis;
    vertScroll = vert;
}

void Canvas::bindEngine(Engine *engine)
{
    this->engine = engine;
}

QPoint Canvas::getDisplayOffset() {
    return QPoint(display_offset_x, display_offset_y);
}

QPoint Canvas::getCanvasSize()
{
    QPoint buf(image->width(), image->height());
    return buf;
}


int32_t Canvas::getSizeOfGex()
{
    return sizeOfGex;
}

int16_t Canvas::getThickness()
{
    return thickness;
}






/*---------Обработчик события мыши. Так и не определился, что лучше - по отпусканию или по нажатию?-----------*/


void Canvas::resizeAndMove(QRect rect) {

    rect.getRect(&anchor_x, &anchor_y, &size_x, &size_y);
    #pragma pack(push,1)
    auto buf = image;
    image = new QImage(QSize(size_x,size_y), QImage::Format_RGB32);
    delete buf;
    #pragma pack(pop)
    this->setGeometry(rect);
    this->size_x = this->width();
    this->size_y = this->height();

}



void Canvas::fillCanvas() {
    memset(image->bits(), 255, image->height() * image->width() * image->depth() / 8);
}

uint32_t Canvas::getPixelColor(int32_t x, int32_t y) {
    if (x < 0 || y < 0 || x >= size_x || y >= size_y)
        return 0xFFFFFFFF;

    uint32_t offset = x *depth()/8 + y*image->bytesPerLine();
    uint32_t buf_color = 0;
    buf_color = image->bits()[offset] << 16 | image->bits()[offset + 1] << 8 | image->bits()[offset + 2];
    return buf_color;
}

void Canvas::setPixel(int32_t x, int32_t y, uint32_t color){
    if (x < 0 || y < 0 || x >= image->width() || y >= image->height())
        return;
    uint32_t offset = x *depth()/8 + y * image->bytesPerLine();
    image->bits()[offset] = color >> 16 & 0xFF;
    image->bits()[offset + 1] = color >> 8 & 0xFF;
    image->bits()[offset + 2] = color & 0xFF;
    this->repaint();
}

void Canvas::storePixel(int32_t x, int32_t y, uint32_t color){
   // if  (x < 0 || y < 0 || x >= size_x || y >= size_y)
    if (x < 0 || y < 0 || x >= image->width() || y >= image->height())
        return;

    uint32_t offset = x *depth()/8 + y * image->bytesPerLine();
    image->bits()[offset] = color >> 16 & 0xFF;
    image->bits()[offset + 1] = color >> 8 & 0xFF;
    image->bits()[offset + 2] = color & 0xFF;
}

void Canvas::storePoint(uint32_t x, uint32_t y, uint32_t color, uint16_t thickness){
    if  (x < 0 || y < 0 || x > size_x || y > size_y)
        return;
    int16_t offset = ((thickness - 1) / 2) * -1;
    int16_t start_x= x + offset;
    int16_t start_y = y + offset;

    for (uint16_t i = 0; i < thickness; i++) {
        for (uint16_t j = 0; j < thickness; j++) {
            storePixel(start_x + i, start_y + j, color);
        }
    }
}



void Canvas::brLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
  const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
  if(steep)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }

  if(x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  const double dx = x2 - x1;
  const double dy = fabs(y2 - y1);

  double error = dx / 2.0f;
  const int ystep = (y1 < y2) ? 1 : -1;
  int y = (int)y1;

  const int maxX = (int)x2;

  for(int x=(int)x1; x<maxX; x++)
  {
    if(steep)
    {
        storePixel(y,x, color);
    }
    else
    {
        storePixel(x,y, color);
    }

    error -= dy;
    if(error < 0)
    {
        y += ystep;
        error += dx;
    }
  }
  this->repaint();
}

void Canvas::drawLine(int x1, int y1, int x2, int y2, uint32_t thickness, uint32_t color) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    //
    int error = deltaX - deltaY;
    //
    storePoint(x2, y2, color, thickness);
    while(x1 != x2 || y1 != y2)
   {
        storePoint(x1, y1, color, thickness);
        const int error2 = error * 2;
        //
        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }

}


void Canvas::fillWithGex(uint16_t sizeOfGex, uint16_t thickness, std::map<std::pair<int32_t,int32_t>, std::pair<double, bool>> cells, QPoint cell_amount) {

    int32_t vert_lines_to_draw = cell_amount.y();
    int32_t horis_lines_to_draw = cell_amount.x();




    this->sizeOfGex = sizeOfGex;
    this->thickness = thickness;
    uint32_t offset_y = (thickness / 2) + (sizeOfGex / 2);
    double buf = (double)(COS60) * (double)sizeOfGex;
    uint32_t offset_x = (double)(COS60) * (double)sizeOfGex;
    gridWidth = offset_x * 2;
    gridHeight = sizeOfGex + offset_y;
    halfWidth = offset_x;
    halfHeight = offset_y;

    if ((halfHeight + sizeOfGex) * cell_amount.y() - display_offset_y < this->height()) {
        vert_lines_to_draw = cell_amount.y() + 1;// - (display_offset_y / (halfHeight + sizeOfGex)) + 1;
    } else
        vert_lines_to_draw = cell_amount.y() + 1;//(int32_t)(size_y / (sizeOfGex + halfHeight) + 10);

    if ((halfWidth * 2) * cell_amount.x() - display_offset_x < this->width()) {
        horis_lines_to_draw = cell_amount.x() + 1;// - (display_offset_x / (halfWidth * 2)) + 1;
    } else
        horis_lines_to_draw = cell_amount.x() + 1;//(int)(size_x / (2 * halfWidth) + 10);

    if (buf);
//size_y / (sizeOfGex * 2) + 2
    for (int32_t j = -2; j < vert_lines_to_draw; j++) {
        int32_t counter_offset_y = 0;
        int32_t counter_offset_x = 0;
        int32_t d2 = 0;
        if (j % 2 == 1) {
            d2 = 0;
            counter_offset_x = offset_x * -1;
        } else {
            d2 = 1;
            counter_offset_x = 0;
        }

        if (j == -1)
            counter_offset_x = offset_x * -1;
        if (j == -2)
            counter_offset_x = 0;

        counter_offset_y = (j * (sizeOfGex + offset_y));



        for (int i = -2; i < horis_lines_to_draw; i++) {

            int32_t x_s = (i * 2 * offset_x) + counter_offset_x - display_offset_x;// + display_offset_x % (halfWidth * 2);
            int32_t y_s = offset_y + counter_offset_y - display_offset_y;// + display_offset_y % (2 * halfHeight + sizeOfGex);

            int32_t x_l = ((i * 2) * offset_x + offset_x) + counter_offset_x - display_offset_x;// + display_offset_x % (halfWidth * 2);
            int32_t y_l = thickness / 2 + 1 + counter_offset_y - display_offset_y;// + display_offset_y % (2 * halfHeight + sizeOfGex);

            int32_t x_f = ((i + 1) * 2 * offset_x) + counter_offset_x- display_offset_x;// + display_offset_x % (halfWidth * 2);
            int32_t y_f = offset_y + counter_offset_y - display_offset_y;// + display_offset_y % (2 * halfHeight + sizeOfGex);

            drawLine(x_s, y_s, x_l, y_l, thickness, 0x00000000);
            if (!(d2 && i - 1 == horis_lines_to_draw))
            drawLine(x_l, y_l, x_f, y_f, thickness, 0x00000000);
        }
    }
//
    for (int32_t i = -2; i < vert_lines_to_draw - 1; i++) {

        int32_t counter_offset_y = 0;
        int32_t counter_offset_x = 0;
        int8_t d2 = 0;
        if (i % 2 == 1) {
            d2 = 1;
            counter_offset_x = offset_x * -1;
        } else {
            d2 = 0;
            counter_offset_x = 0;
        }

        if (i == -1)
            counter_offset_x = offset_x * -1;
        if (i == -2)
            counter_offset_x = 0;

        counter_offset_y = (i * (sizeOfGex + offset_y));
        for (int32_t j = -2; j < horis_lines_to_draw + d2; j++) {
            int32_t x_s = (j * 2 * offset_x) + counter_offset_x - display_offset_x;// + display_offset_x % (halfWidth * 2);
            int32_t y_s = offset_y + counter_offset_y - display_offset_y;// + display_offset_y % (2 * halfHeight + sizeOfGex);

            int32_t x_l = (j * 2 * offset_x) + counter_offset_x - display_offset_x;// + display_offset_x % (halfWidth * 2);
            int32_t y_l = thickness / 2 + 1 + counter_offset_y + offset_y + sizeOfGex - display_offset_y;// + display_offset_y % (2 * halfHeight + sizeOfGex);

            drawLine(x_s, y_s, x_l, y_l, thickness ,0x00000000);
        }
    }
    fillGexes(cells);
}



Hex_number Canvas::getSelectedHexagon(int x, int y)
{
    // Find the row and column of the box that the point falls in.
    int row = (int) (y / gridHeight);
    int column;
    double m = (double)halfHeight / (double)halfWidth;
    bool rowIsOdd = row % 2 == 1;

    // Is the row an odd number?
    if (rowIsOdd)// Yes: Offset x to match the indent of the row
        column = (int) ((x - halfWidth) / gridWidth);
    else// No: Calculate normally
        column = (int) (x / gridWidth);
    // Work out the position of the point relative to the box it is in
    double relY = y - (row * gridHeight);
    double relX = 0;



    if (rowIsOdd)
        relX = (x - (column * gridWidth)) - halfWidth;
    else
        relX = x - (column * gridWidth);
    double biba = (m * relX) - (double)halfHeight;


    // Work out if the point is above either of the hexagon's top edges
    if (relY < (-m * relX) + (double)halfHeight) // LEFT edge
        {
            row--;
            if (!rowIsOdd)
                column--;
        }
    else if (relY < (m * relX) - (double)halfHeight) // RIGHT edge
        {
            row--;
            if (rowIsOdd)
                column++;
        }
    if (_DEBUG) {
        QMessageBox msgBox;
        char buf[40] = {0};
        sprintf(buf, "%d %d %lf %lf",column,row,relX, relY);
        msgBox.setText(buf);
        msgBox.exec();
    }

    Hex_number hex;

    if (column < 0 || row < 0 || relX < 0 || relY < 0)
        hex.valid = INVALID;
    else
        hex.valid = VALID;
    hex.x = column;
    hex.y = row;
    return hex;
}

struct Point {
    int32_t x;
    int32_t y;
};

void Canvas::floodFillScanlineStack(int x_INVERTED, int y_INVERTED, uint32_t newColor, uint32_t oldColor)
{
    std::stack<Point> ff_stack;
  if(oldColor == newColor) return;

  int x1;
  bool spanAbove, spanBelow;

  struct Point point;
  point.x = x_INVERTED;
  point.y = y_INVERTED;

  ff_stack.push(point);

  while(!ff_stack.empty())
  {
     struct Point p = ff_stack.top();
     ff_stack.pop();
    x1 = p.x;
    while(x1 >= 0 && getPixelColor(p.y, x1) == oldColor) x1--;
    x1++;
    spanAbove = spanBelow = 0;
    while(x1 < image->height() && getPixelColor(p.y, x1) == oldColor )
    {
        storePixel(p.y, x1, newColor);
      if(!spanAbove && p.y > 0 && getPixelColor(p.y - 1, x1) == oldColor)
      {
          struct Point pp;
          pp.x = x1;
          pp.y = p.y - 1;
        ff_stack.push(pp);
        spanAbove = 1;
      }
      else if(spanAbove && y_INVERTED > 0 && getPixelColor(p.y - 1, x1) != oldColor)
      {
        spanAbove = 0;
      }
      if(!spanBelow && p.y < image->width() - 1 && getPixelColor(p.y + 1, x1) == oldColor)
      {
          struct Point pp;
          pp.x = x1;
          pp.y = p.y + 1;
        ff_stack.push(pp);
        spanBelow = 1;
      }
      else if(spanBelow && p.y < image->width() - 1 && getPixelColor(p.y + 1, x1) != oldColor)
      {
        spanBelow = 0;
      }
      x1++;
    }
  }
 // this->repaint();
}

void Canvas::fillAt(int32_t x, int32_t y, uint32_t color) {

}

void Canvas::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::MidButton) {
        mouse_states.mmb = true;
        mouse_states.prev_x = event->x();
        mouse_states.prev_y = event->y();
    } else
    if (event->button() == Qt::LeftButton) {
        //floodFillScanlineStack(event->y(),event->x(), 0x0000FF00, 0x00FFFFFF);
        auto prev = getSelectedHexagon(event->x()  + display_offset_x, event->y() + display_offset_y);
        start_xor_x = prev.x;
        start_xor_y = prev.y;
        mouse_states.lmb = true;
    } else
    if (event->button() == Qt::RightButton) {
        mouse_states.rmb = true;
    }


}

void Canvas::repaintCells() {
    auto cells = this->engine->getCells();
        fillCanvas();
        fillWithGex(sizeOfGex, thickness, cells, QPoint(engine->columns, engine->rows));
}

void Canvas::setDisplayOffset(int32_t x, int32_t y) {
    if (x >= 0) {
        display_offset_x = x;
        repaintCells();
    }

    if (y >= 0) {
        display_offset_y = y;
        repaintCells();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton) {
        return;
        if (display_offset_x - (event->x() - mouse_states.prev_x) < 0) {
            display_offset_x = 0;
        } else
            display_offset_x -= (event->x() - mouse_states.prev_x);

        if (display_offset_y - (event->y() - mouse_states.prev_y) < 0) {
            display_offset_y = 0;
        } else
            display_offset_y -= (event->y() - mouse_states.prev_y);

        mouse_states.mmb = false;

        int32_t drawing_x = halfWidth * 2 * engine->columns + halfWidth;
        int32_t drawing_y = (halfHeight + getSizeOfGex()) * engine->rows + halfHeight;

        if (drawing_x < width())
            horisScroll->setEnabled(false);
        else
            horisScroll->setEnabled(true);

        if (drawing_y < height())
            vertScroll->setEnabled(false);
        else
            vertScroll->setEnabled(true);

        horisScroll->setMaximum(abs(width() - drawing_x - 1));
        vertScroll->setMaximum(abs(height() - drawing_y - 1));

        repaintCells();
    } else
    if (event->button() == Qt::LeftButton) {
        auto cur = getSelectedHexagon(event->x()  + display_offset_x, event->y() + display_offset_y);
        if (cur.x == last_xor_x && cur.y == last_xor_y) {
           // if (last_xor_x == start_xor_x && last_xor_y == start_xor_y) {
            last_xor_x = -20;
            last_xor_y = -20;
                mouse_states.lmb = false;
                return;
          //  }
        }
        if (cur.x == start_xor_x && cur.y == start_xor_y) {
            if (engine->getCell(cur.x, cur.y)) {
                if (engine->resetCell(cur.x, cur.y)) {
                    floodFillScanlineStack(event->y(),event->x(), 0x00FFFFFF, 0x0000FF00);
                    this->repaint();
                }
            } else
            if (engine->setCell(cur.x, cur.y)) {
                floodFillScanlineStack(event->y(),event->x(), 0x0000FF00, 0x00FFFFFF);
                this->repaint();
            }
        }
        mouse_states.lmb = false;
    } else
    if (event->button() == Qt::RightButton) {
        mouse_states.rmb = false;
        if (getPixelColor(event->x(), event->y()) == 0x00000000)
            return;
        Hex_number buf = getSelectedHexagon(event->x() + display_offset_x, event->y() + display_offset_y);
        if (engine->setCell(buf.x, buf.y)) {
            floodFillScanlineStack(event->y(),event->x(), 0x0000FF00, 0x00FFFFFF);
            this->repaint();
        }

    }
//    if (event->button() == Qt::LeftButton) {
//        std::pair<int,int> buf_pair = controller->getCellFromClick(event->pos().rx(),event->pos().ry());
//            cells[std::make_pair(buf_pair.first,buf_pair.second)] ?
//                 cells[std::make_pair(buf_pair.first,buf_pair.second)] = DEAD :
//                    cells[std::make_pair(buf_pair.first,buf_pair.second)] = ALIVE;
//            this->repaint();
//    }

}


#include <QThread>
/*-------------------------------Отрисовка всего поля------------------------------------*/
void Canvas::paintEvent(QPaintEvent * ) {
  //  image.load("C:\\alive.jpg");
//    image.bits()[i] = 0x00;
    QPainter painter(this); // определяем объект painter, который обеспечивает рисование
   // painter.
    painter.drawImage(QRect(0, 0, image->size().width(), image->size().height()), *image); // рисуем наше изображение от 0,0 и растягиваем по всему виджет
    if (!showImpacts)
        return;
    painter.setPen(QPen(Qt::black));
    painter.setFont(QFont("Times", sizeOfGex - 4, QFont::Normal));
    int decOff = 0;
    int dx = -1 * (display_offset_x);
    int dy = -1 * (display_offset_y);
    auto cells = engine->getCells();
    for (int32_t x = 0; x < engine->columns; x++) {
        for (int32_t y = 0; y < engine->rows; y++) {
            if (y % 2)
                decOff = halfWidth;
            else decOff = 0;
            painter.drawText(QRect(dx + (halfWidth * 2) * x + decOff,
                                   dy + (sizeOfGex + halfHeight) * y,
                                   halfWidth * 2, sizeOfGex + 2 * halfHeight), Qt::AlignCenter, QString::number(cells[std::make_pair(x, y)].first));
            //QThread::msleep(1000);
        }
    }

   // myLabel->setPixmap(QPixmap::fromImage(image));
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
    if (mouse_states.rmb) {
        if (getPixelColor(event->x(), event->y()) == 0x00000000)
            return;
        Hex_number buf = getSelectedHexagon(event->x() + display_offset_x, event->y() + display_offset_y);
        if (buf.x == prev_hex.x && buf.y == prev_hex.y) {
            return;
        } else {
            prev_hex.x = buf.x;
            prev_hex.y = buf.y;
            if (engine->setCell(buf.x, buf.y)) {
                floodFillScanlineStack(event->y(),event->x(), 0x0000FF00, 0x00FFFFFF);
                this->repaint();
            }
        }
    } else if (mouse_states.lmb) {
        if (getPixelColor(event->x(), event->y()) == 0x00000000)
            return;
        Hex_number cur = getSelectedHexagon(event->x() + display_offset_x, event->y() + display_offset_y);
        if (cur.x == start_xor_x && cur.y == start_xor_y) {
            return;
        } else {
            start_xor_x = cur.x;
            start_xor_y = cur.y;
            last_xor_x = cur.x;
            last_xor_y = cur.y;
            if (engine->getCell(cur.x, cur.y)) {
                if (engine->resetCell(cur.x, cur.y)) {
                    floodFillScanlineStack(event->y(),event->x(), 0x00FFFFFF, 0x0000FF00);
                    this->repaint();
                }
            } else
            if (engine->setCell(cur.x, cur.y)) {
                floodFillScanlineStack(event->y(),event->x(), 0x0000FF00, 0x00FFFFFF);
                this->repaint();
            }
        }
    }
}

void Canvas::fillGexes(std::map<std::pair<int32_t, int32_t>, std::pair<double, bool>> cells)
{
    int32_t start_pos_y = -display_offset_y + (sizeOfGex / 2) + halfHeight;
    int32_t start_pos_x = -display_offset_x + halfWidth;
    for (int32_t j = 0; j < engine->columns; j++) {
        for (int32_t i = 0; i < engine->rows; i++) {
            if ((i % 2)) {
                start_pos_x += halfWidth;
            }
            auto pair = std::make_pair(j, i);
            if (cells[pair].second) {
                int32_t x_off = start_pos_x;
                int32_t y_off = start_pos_y;
//                QMessageBox msgBox;
//                char buf[40] = {0};
//                sprintf(buf, "%d %d",x_off + halfWidth, y_off + ((sizeOfGex / 2) + halfHeight));
//                msgBox.setText(buf);
//                msgBox.exec();
                if (x_off < 0 && getPixelColor(0, y_off) != 0x00000000)
                {
                    if (x_off + halfWidth < 0)
                        x_off = -1;
                    else
                    x_off = 0;
                }

                if (x_off > this->width() && getPixelColor(this->width(), y_off) != 0x00000000) //kostil
                {
                    if (x_off - halfWidth + 1 > this->width())
                        x_off = -1;
                    else
                    x_off = this->width() - 1;
                }

                if (y_off < 0 && getPixelColor(x_off, 0) != 0x00000000)
                {
                    if (y_off + (sizeOfGex / 2) + halfHeight < 0)
                        y_off = -10;
                    else
                    y_off = 0;
                }

                if (y_off > this->height() && getPixelColor(x_off, this->height()) != 0x00000000) //velosiped
                {
                    if (y_off - ((sizeOfGex / 2) + halfHeight) + 3 > this->height())
                        y_off = -10;
                    else
                    y_off = this->height() - 1;
                }

                floodFillScanlineStack(y_off, x_off, 0x0000FF00, 0x00FFFFFF);
               // setPixel(x_off, y_off, 0x000000ff);
            }
            if ((i % 2)) {
                start_pos_x -= halfWidth;
            }
            start_pos_y += (halfHeight + sizeOfGex);
        }
        start_pos_y = -display_offset_y + (sizeOfGex / 2) + halfHeight;

        start_pos_x += 2*(halfWidth);
    }
}

/*---------Обработчик колеса мыши. Используется для масштабирования поля. Думается мне это точно в VIEW.-----------*/
void Canvas::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical) {
               if (event->delta() < 0) {
                   if (sizeOfGex >= 7)
                       this->sizeOfGex-= 1;
               } else
                   if (sizeOfGex <= 40)
                       this->sizeOfGex+= 1;

    }

    fillCanvas();
    auto test_buf = engine->getCells();
    fillWithGex(getSizeOfGex(),getThickness(),test_buf,QPoint(engine->columns, engine->rows));

    int32_t drawing_x = halfWidth * 2 * engine->columns + halfWidth;
    int32_t drawing_y = (halfHeight + getSizeOfGex()) * engine->rows + halfHeight;

    if (drawing_x < width())
        horisScroll->setEnabled(false);
    else
        horisScroll->setEnabled(true);

    if (drawing_y < height())
        vertScroll->setEnabled(false);
    else
        vertScroll->setEnabled(true);

    horisScroll->setMaximum(abs(width() - drawing_x - 1));
    vertScroll->setMaximum(abs(height() - drawing_y - 1));
}
