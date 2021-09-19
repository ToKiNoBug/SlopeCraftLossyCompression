#include "lossyCompressor.h"

#ifdef WITH_QT
LossyCompressor::LossyCompressor(QObject *parent) : QObject(parent)
#else
LossyCompressor::LossyCompressor()
#endif
{

}
