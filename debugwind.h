#ifndef DEBUGWIND_H
#define DEBUGWIND_H

#include <QMainWindow>

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

private:
    Ui::DebugWind *ui;
};
#endif // DEBUGWIND_H
