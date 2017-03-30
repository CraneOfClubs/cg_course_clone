#include "engine.h"

double correctRound( double val ) {
    return round( val * 10.0 ) / 10.0;
}


double Engine::getImpact(int32_t i, int32_t j) {
    double sum;
    if (j % 2) {
        sum= (*cells)[std::make_pair(i + 1,j - 1)].second * _settings.firstImpact +
               (*cells)[std::make_pair(i,j - 1)].second * _settings.firstImpact +
                (*cells)[std::make_pair(i,j + 1)].second * _settings.firstImpact +
                (*cells)[std::make_pair(i + 1,j + 1)].second * _settings.firstImpact +
                (*cells)[std::make_pair(i + 1,j)].second * _settings.firstImpact +
                (*cells)[std::make_pair(i - 1,j)].second * _settings.firstImpact +
                (*cells)[std::make_pair(i,j + 2)].second * _settings.secondImpact +
                (*cells)[std::make_pair(i,j - 2)].second * _settings.secondImpact +
                (*cells)[std::make_pair(i - 1,j + 1)].second * _settings.secondImpact +
                (*cells)[std::make_pair(i + 2,j + 1)].second * _settings.secondImpact +
                (*cells)[std::make_pair(i - 1,j - 1)].second * _settings.secondImpact +
                (*cells)[std::make_pair(i + 2,j - 1)].second * _settings.secondImpact;
    } else {
        sum= (*cells)[std::make_pair(i - 1,j - 1)].second * _settings.firstImpact +
               (*cells)[std::make_pair(i,j - 1)].second * _settings.firstImpact +
                (*cells)[std::make_pair(i, j + 1)].second * _settings.firstImpact +
                (*cells)[std::make_pair(i - 1, j + 1)].second * _settings.firstImpact +
                (*cells)[std::make_pair(i + 1, j)].second * _settings.firstImpact +
                (*cells)[std::make_pair(i - 1, j)].second * _settings.firstImpact +
                (*cells)[std::make_pair(i,j + 2)].second * _settings.secondImpact +
                (*cells)[std::make_pair(i,j - 2)].second * _settings.secondImpact +
                (*cells)[std::make_pair(i + 1,j + 1)].second * _settings.secondImpact +
                (*cells)[std::make_pair(i - 2,j + 1)].second * _settings.secondImpact +
                (*cells)[std::make_pair(i + 1,j - 1)].second * _settings.secondImpact +
                (*cells)[std::make_pair(i - 2,j - 1)].second * _settings.secondImpact;
    }
    return correctRound(sum);
}

void Engine::reCalcImpacts(){
    for (int i = 0; i < columns ; i++) {
        for (int j = 0; j < columns ; j++) {
            (*cells)[std::make_pair(i,j)].first = getImpact(i, j);
        }
    }
}


bool Engine::setCell(int32_t x, int32_t y)
{
    if (x < columns && y < rows) {
        (*cells)[std::make_pair(x,y)].second = true;
        reCalcImpacts();
        return true;
    } else
        return false;
}

Engine::Engine(Canvas* _view)
{
    cells = new std::map<std::pair<int32_t, int32_t>, std::pair<double,bool>>();
    view = _view;
}

void Engine::loadFromFile(std::string filename)
{
    try {
        int16_t buf_gex_size = 0;
        int16_t buf_thickness = 0;
        int32_t cells_amount = 0;
        int32_t x = 0, y = 0;
        std::pair <int,int> buf_pair;
        QString rot_snoshal_etogo_qt = QString::fromUtf8(filename.c_str());
        QFile file(rot_snoshal_etogo_qt);
        if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", file.errorString());
        }

        QTextStream in(&file);

        QString line = in.readLine();
        QStringList fields = line.split(" ");
        columns = fields[0].toInt();
        rows = fields[1].toInt();

        line = in.readLine();
        buf_thickness = line.toInt();

        line = in.readLine();
        buf_gex_size = line.toInt();

        line = in.readLine();
        cells_amount = line.toInt();


        for (int32_t i = 0; i < cells_amount; i++) {
            line = in.readLine();
            fields = line.split(" ");
            x = fields[0].toInt();
            y = fields[1].toInt();
            buf_pair = std::make_pair(x,y);
            (*cells)[buf_pair].second = true;
        }

        file.close();

        view->fillWithGex(buf_gex_size, buf_thickness, *cells,  QPoint(columns, rows));
    } catch (QException e) {

    }
}

void Engine::saveToFile(std::string filename) {
    QString rot_snoshal_etogo_qt = QString::fromUtf8(filename.c_str());
    QFile file(rot_snoshal_etogo_qt);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QTextStream out(&file);

    out << columns << ' ' << rows << endl;
    out << view->getThickness() << endl;
    out << view->getSizeOfGex() << endl;
    int32_t count = 0;

    for (int32_t i = 0; i < columns; i++) {
        for (int32_t j = 0; j < rows; j++) {
            if ((*cells)[std::make_pair(i, j)].second) {
                count++;
            }
        }
    }
    out << QString::number(count) << endl;

    for (int32_t i = 0; i < columns; i++) {
        for (int32_t j = 0; j < rows; j++) {
            if ((*cells)[std::make_pair(i, j)].second) {
                out << QString::number(i) << " " << QString::number(j) << endl;
            }
        }
    }
    file.close();
}

std::map<std::pair<int32_t, int32_t>, std::pair<double, bool>> Engine::getCells()
{
    return *cells;
}

void Engine::startNewGame(int32_t xsize, int32_t ysize)
{
    columns = xsize;
    rows = ysize;
    cells->clear();
    view->fillCanvas();
    view->fillWithGex(view->getSizeOfGex(),view->getThickness(),*cells,QPoint(columns, rows));
}

bool Engine::step()
{
    auto prev_cells = *cells;

    std::pair <int,int> buf_pair;
    reCalcImpacts();
    for (int i = 0; i < columns ; i++) {
        for (int j = 0; j < columns ; j++) {
            buf_pair = std::make_pair(i,j);
                if (prev_cells[buf_pair].second && (prev_cells[buf_pair].first <= _settings.liveEnd && prev_cells[buf_pair].first >= _settings.liveBegin)) {
                    (*cells)[buf_pair].second = true;
                    continue;
                }
                if (prev_cells[buf_pair].second  && (prev_cells[buf_pair].first <= _settings.liveBegin)) {
                    (*cells)[buf_pair].second  = false;
                    continue;
                }
                if (prev_cells[buf_pair].second && (prev_cells[buf_pair].first >= _settings.liveEnd)) {
                    (*cells)[buf_pair].second  = false;
                    continue;
                }

                if (!prev_cells[buf_pair].second)
                    if (prev_cells[buf_pair].first >= _settings.birthBegin && prev_cells[buf_pair].first <= _settings.birthEnd) {
                    (*cells)[buf_pair].second  = true;
                    continue;
                }
        }
    }
    view->fillCanvas();
    view->fillWithGex(view->getSizeOfGex(),view->getThickness(),*cells,QPoint(columns, rows));
    return true;
}
