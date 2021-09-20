#ifndef DEBUGWIND_H
#define DEBUGWIND_H

#include <QMainWindow>
#include "TokiSlopeCraft.h"
#include "lossyCompressor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DebugWind; }
QT_END_NAMESPACE

class DebugWind : public QMainWindow
{
    Q_OBJECT

public:
    DebugWind(QWidget *parent = nullptr);
    ~DebugWind();

private slots:
    void progressRangeSet(int min,int max,int val);
    void progressAdd(int);
    void keepAwake();
private:
    Ui::DebugWind *ui;
    TokiSlopeCraft * Kernel;
    LossyCompressor * Compressor;
    void initialize();
};
#endif // DEBUGWIND_H
