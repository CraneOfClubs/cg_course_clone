#include "settingswindget.h"


void SettingsWindget::saveSettings()
{
    MainWindow *mw = dynamic_cast<MainWindow*>(_parent);
    mw->getEngine()->setupFuncLimits(x_start->value(), y_start->value(), x_end->value(), x_end->value());
    std::vector<uint32_t> colors;
    for(int32_t i = 0; i < _color_widget->count(); ++i) {
        auto color_i = _color_widget->item(i)->backgroundColor();
        auto color_r = color_i.rgba() << 8 | 0x000000FF;
        colors.push_back(color_r);
        int biba = 0;
    }
    mw->getEngine()->setupColors(colors);
    mw->grid_cols = columns->value();
    mw->grid_rows = rows->value();
    mw->canvas->redraw();
}

void SettingsWindget::addNewColor() {
    QColorDialog c_dialog;
    c_dialog.setModal(true);
    connect(&c_dialog, &QColorDialog::colorSelected, this, [this] (const QColor &color) {
        QListWidgetItem *item = new QListWidgetItem(this->_color_widget);
        item->setBackgroundColor(color);
        this->_color_widget->addItem(item);
    });
    if (this->_color_widget->count() >= 1)
        _remove_color->setEnabled(1);
    c_dialog.exec();
}

void SettingsWindget::removeColors() {
    for (int32_t i = 0; i < _color_widget->selectedItems().size(); ++i)
        delete _color_widget->selectedItems()[i];

    if (this->_color_widget->count() <= 1)
        _remove_color->setEnabled(0);
}

SettingsWindget::SettingsWindget(QWidget *parent)
{
    _parent = parent;
    MainWindow *mw = dynamic_cast<MainWindow*>(_parent);
    grid = new QGridLayout(this);
    ugrid = new QGridLayout(this);
    grid->addLayout(ugrid, 1, 0);
    QLabel * plb = new QLabel(tr("Function limits:"), this);
    grid->addWidget(plb, 0,0, Qt::AlignCenter);

    QLabel * x_s_l = new QLabel(tr("X Starts at:"), this);
    ugrid->addWidget(x_s_l, 1,1);
    x_start = new QDoubleSpinBox(this);
    x_start->setMinimum(-1000);
    x_start->setMaximum(1000);
    x_start->setValue(-10);
    ugrid->addWidget(x_start, 2, 1);

    QLabel * x_e_l = new QLabel(tr("X Ends at:"), this);
    ugrid->addWidget(x_e_l, 3,1);
    x_end = new QDoubleSpinBox(this);
    x_end->setMinimum(-1000);
    x_end->setMaximum(1000);
    x_end->setValue(10);
    ugrid->addWidget(x_end, 4, 1);

    QLabel * y_s_l = new QLabel(tr("Y Starts at:"), this);
    ugrid->addWidget(y_s_l, 1,2);
    y_start = new QDoubleSpinBox(this);
    y_start->setMinimum(-1000);
    y_start->setMaximum(1000);
    y_start->setValue(-10);
    ugrid->addWidget(y_start, 2, 2);

    QLabel * y_e_l = new QLabel(tr("Y Ends at:"), this);
    ugrid->addWidget(y_e_l, 3,2);
    y_end = new QDoubleSpinBox(this);
    y_end->setMinimum(-1000);
    y_end->setMaximum(1000);
    y_end->setValue(10);
    ugrid->addWidget(y_end, 4, 2);

    QLabel * rows_l = new QLabel(tr("Grid rows:"), this);
    ugrid->addWidget(rows_l, 5,1);
    rows = new QSpinBox(this);
    rows->setMinimum(0);
    rows->setMaximum(100);
    rows->setValue(40);
    ugrid->addWidget(rows, 5, 2);

    QLabel * cols_l = new QLabel(tr("Grid columns:"), this);
    ugrid->addWidget(cols_l, 6,1);
    columns = new QSpinBox(this);
    columns->setMinimum(0);
    columns->setMaximum(100);
    columns->setValue(40);
    ugrid->addWidget(columns, 6,2);

    QLabel * color_l = new QLabel(tr("Color list:"), this);
    grid->addWidget(color_l, 2,0, Qt::AlignCenter);
    _color_widget = new QListWidget(this);

    _color_widget->setDragDropMode(QAbstractItemView::DragDrop);
    _color_widget->setDefaultDropAction(Qt::MoveAction);
    _color_widget->setSelectionMode(QAbstractItemView::SingleSelection);

    auto levels = mw->getEngine()->getAllColors();
    for (auto it = levels.begin(); it < levels.end(); ++it) {
        QListWidgetItem *color = new QListWidgetItem(this->_color_widget);
        color->setBackgroundColor(QColor::fromRgb(*it >> 8));
        this->_color_widget->addItem(color);
    };

    grid->addWidget(_color_widget, 3,0);

    _apply = new QPushButton("Apply", this);
    grid->addWidget(_apply, 6, 0);

    _add_color = new QPushButton("Add color", this);
    grid->addWidget(_add_color, 4, 0);

    _remove_color = new QPushButton("Remove color", this);
    grid->addWidget(_remove_color, 5, 0);

    connect(_apply, SIGNAL(released()), this, SLOT(saveSettings()));
    connect(_add_color, SIGNAL(released()), this, SLOT(addNewColor()));
    connect(_remove_color, SIGNAL(released()), this, SLOT(removeColors()));
}
