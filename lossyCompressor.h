#ifndef LOSSYCOMPRESSOR_H
#define LOSSYCOMPRESSOR_H

#include <iostream>
#include <vector>
#include <map>
#include <Eigen/Dense>

#include "WaterItem.h"
#include "ColorSet.h"
#include "OptiChain.h"

#define WITH_QT

#ifdef WITH_QT
    #include <QObject>
#endif

using namespace Eigen;


#ifdef WITH_QT
class LossyCompressor : public QObject
{
    Q_OBJECT
public:
    explicit LossyCompressor(QObject *parent = nullptr);
#else
class LossyCompressor
{
public:
    LossyCompressor();
#endif

    void setSource(const ArrayXi & ,const TokiColor *[],ushort col);
    bool compress(ushort maxHeight,bool allowNaturalCompress);
    const ArrayXi & getBase() const;
    const ArrayXi & getHighLine() const;
    const ArrayXi & getLowLine() const;
    const std::map<TokiPos,waterItem> & getWaterList() const;
#ifdef WITH_QT
signals:
    void progressRangeSet(int min,int max,int val);
    void progressAdd(int);
    void keepAwake();
#endif

private:
    std::vector<const TokiColor*> source;
    ArrayXi mapColor;
    ushort col;
    std::map<TokiPos,waterItem> waterList;

};

#endif // LOSSYCOMPRESSOR_H
