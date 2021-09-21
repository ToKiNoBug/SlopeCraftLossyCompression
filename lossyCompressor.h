/*
 Copyright © 2021  TokiNoBug
This file is part of SlopeCraft.

    SlopeCraft is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SlopeCraft is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SlopeCraft.  If not, see <https://www.gnu.org/licenses/>.

    Contact with me:
    github:https://github.com/ToKiNoBug
    bilibili:https://space.bilibili.com/351429231
*/


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
    void caculateFitness(const TokiColor**,ushort maxHeight,bool allowNaturalCompress);
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
    static ushort maxGeneration;
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
