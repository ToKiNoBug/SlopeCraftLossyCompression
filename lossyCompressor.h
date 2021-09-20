#ifndef LOSSYCOMPRESSOR_H
#define LOSSYCOMPRESSOR_H


#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <random>
#include <Eigen/Dense>

#include "WaterItem.h"
#include "ColorSet.h"
#include "HeightLine.h"

#define WITH_QT

#ifdef WITH_QT
    #include <QObject>
#endif

class gene
{
public:
    gene();
    gene(ushort);


    bool isCaculated() const;
    float getFitness() const;
    ushort size() const;
    const Eigen::Array<uchar,Eigen::Dynamic,1> & getDNA() const;

    void initialize(ushort size);
    void caculateFitness(const TokiColor*[],ushort maxHeight,bool allowNaturalCompress);
    static void crossover(gene* ,gene*,ushort idx);
    void mutate(ushort idx);
    //void setDNAValue(uchar val,ushort idx);
private:
    float fitness;
    Eigen::Array<uchar,Eigen::Dynamic,1> DNA;

    static const float unCaculatedSign;
    static const uchar mutateMap[3][2];
};

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

    void setSource(const Eigen::ArrayXi & ,const TokiColor *[]);
    bool compress(ushort maxHeight,bool allowNaturalCompress=false);
    const gene& getResult() const;
#ifdef WITH_QT
signals:
    void progressRangeSet(int min,int max,int val);
    void progressAdd(int);
    void keepAwake();
#else
    void (*progressRangeSet)(int min,int max,int val);
    void (*progressAdd)(int);
    void (*keepAwake)();
#endif

private:
    std::vector<const TokiColor*> source;
    std::vector<gene> population;
    bool allowNaturalCompress;
    ushort maxHeight;
    ushort eliteIdx;
    ushort generation;
    ushort failTimes;

    static const ushort popSize;
    static const ushort maxFailTimes;
    static const ushort maxGeneration;
    static const double crossoverProb;
    static const double mutateProb;
    static const double initializeNonZeroRatio;

    void runGenetic();

    void initialize();
    void caculateFitness();
    void select();
    void crossover();
    void mutate();
};

double randD();
#endif // LOSSYCOMPRESSOR_H
