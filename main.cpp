#include "debugwind.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DebugWind w;
    w.show();
    return a.exec();
}
