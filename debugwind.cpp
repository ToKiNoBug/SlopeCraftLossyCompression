#include "debugwind.h"
#include "ui_debugwind.h"

DebugWind::DebugWind(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DebugWind)
{
    ui->setupUi(this);
    Kernel=new TokiSlopeCraft(this);
    Compressor=new LossyCompressor(this);

    connect(Compressor,&LossyCompressor::progressRangeSet,
            this,&DebugWind::progressRangeSet);
    connect(Compressor,&LossyCompressor::progressAdd,
            this,&DebugWind::progressAdd);
    connect(Compressor,&LossyCompressor::keepAwake,
            this,&DebugWind::keepAwake);
}

DebugWind::~DebugWind()
{
    delete Kernel;
    delete Compressor;
    delete ui;
}

void DebugWind::progressRangeSet(int min,int max,int val) {
    ui->progressBar->setRange(min,max);
    ui->progressBar->setValue(val);
}

void DebugWind::progressAdd(int delta) {
    ui->progressBar->setValue(
                ui->progressBar->value()+delta);
}

void DebugWind::keepAwake() {
    QApplication::processEvents();
}

void DebugWind::initialize() {

}
