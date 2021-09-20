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

    void initialize();
private slots:
    void progressRangeSet(int min,int max,int val);
    void progressAdd(int);
    void keepAwake();
    void on_BtnLoad_clicked();

    void on_BtnCompress_clicked();

private:
    Ui::DebugWind *ui;
    TokiSlopeCraft * Kernel;
    LossyCompressor * Compressor;
    std::vector<const TokiColor*>currentCol;

    HeightLine HL;
};

EImage QImage2EImage(const QImage&);
QImage EImage2QImage(const EImage & ei,ushort=1);
#endif // DEBUGWIND_H
