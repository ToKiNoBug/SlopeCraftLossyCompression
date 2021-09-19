#include "debugwind.h"
#include "ui_debugwind.h"

DebugWind::DebugWind(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DebugWind)
{
    ui->setupUi(this);
}

DebugWind::~DebugWind()
{
    delete ui;
}

