QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += D:/eigen-3.3.9 \
                               D:/Git/SlopeCraft/Kernel

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#DEFINES += WIN32_LEAN_AND_MEAN

SOURCES += \
    ../SlopeCraft/Kernel/CIEDE00.cpp \
    ../SlopeCraft/Kernel/HeightLine.cpp \
    ../SlopeCraft/Kernel/OptiChain.cpp \
    ../SlopeCraft/Kernel/WaterItem.cpp \
    ../SlopeCraft/Kernel/colorset.cpp \
    ../SlopeCraft/Kernel/tokicolor.cpp \
    lossyCompressor.cpp \
    main.cpp \
    debugwind.cpp

HEADERS += \
    ../SlopeCraft/Kernel/ColorSet.h \
    ../SlopeCraft/Kernel/HeightLine.h \
    ../SlopeCraft/Kernel/OptiChain.h \
    ../SlopeCraft/Kernel/TokiColor.h \
    ../SlopeCraft/Kernel/WaterItem.h \
    debugwind.h \
    lossyCompressor.h

FORMS += \
    debugwind.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
