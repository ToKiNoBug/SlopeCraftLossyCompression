#include "debugwind.h"

#include <ctime>
#include <QApplication>

int main(int argc, char *argv[])
{
    std::srand(std::time(NULL));
    QApplication a(argc, argv);
    DebugWind w;
    w.initialize();
    w.show();
    return a.exec();
}
