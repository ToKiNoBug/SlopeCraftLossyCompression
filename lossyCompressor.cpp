#include "lossyCompressor.h"

const float gene::unCaculatedSign=-65536*2;
const uchar gene::mutateMap[3][2]={{1,2},{0,2},{0,1}};

const ushort LossyCompressor::popSize=50;
const ushort LossyCompressor::maxFailTimes=30;
const ushort LossyCompressor::maxGeneration=200;
const double LossyCompressor::crossoverProb=0.9;
const double LossyCompressor::mutateProb=0.01;
const double LossyCompressor::initializeNonZeroRatio=0.05;

double randD(){
    static std::default_random_engine generator(time(0));
    static std::uniform_real_distribution<double> rander(0,1);
    return rander(generator);
}

gene::gene() {
    fitness=unCaculatedSign;
}

gene::gene(ushort size) {
    DNA.setZero(size);
    fitness=unCaculatedSign;
}

bool gene::isCaculated() const {
    return fitness>unCaculatedSign;
}

float gene::getFitness() const {
    return fitness;
}

ushort gene::size() const {
    return DNA.size();
}

const Eigen::Array<uchar,Eigen::Dynamic,1> & gene::getDNA() const {
    return DNA;
}
void gene::initialize(ushort size) {
    DNA.setZero(size);
}
void gene::caculateFitness(const TokiColor** src,ushort maxHeight) {
    if(isCaculated())return;

    HeightLine HL;
    float sumColorDiff=HL.make(src,DNA);
    if(HL.maxHeight()>maxHeight) {
        fitness=maxHeight-HL.maxHeight();
    } else {
        fitness=100.0/(1e-3f+sumColorDiff);
    }
}
void gene::crossover(gene* a,gene* b,ushort idx) {
    a->DNA.segment(0,idx).swap(b->DNA.segment(0,idx));
    a->fitness=unCaculatedSign;
    b->fitness=unCaculatedSign;
    // 0<idx<length-1
}
void gene::mutate(ushort idx) {
    DNA(idx)=mutateMap[DNA(idx)][rand()%2];
    fitness=unCaculatedSign;
}

#ifdef WITH_QT
LossyCompressor::LossyCompressor(QObject *parent) : QObject(parent)
#else
LossyCompressor::LossyCompressor()
#endif
{
    population.resize(popSize);
}

void LossyCompressor::initialize() {
    population.resize(popSize);
    for(ushort i=0;i<popSize;i++) {
        population[i].initialize(source.size());
        for(ushort r=0;r<source.size();r++) {
            if(randD()<initializeNonZeroRatio)
                population[i].mutate(r);
        }
    }
}

void LossyCompressor::caculateFitness() {
    for(ushort i=0;i<popSize;i++)
        population[i].caculateFitness(&source[0],maxHeight);
}

void LossyCompressor::select() {
    ushort minIdx=0,maxIdx=0;
    float minFitness=population[0].getFitness(),
            maxFitness=population[0].getFitness();
    float temp;
    for(ushort idx=0;idx<popSize;idx++) {
        temp=population[idx].getFitness();
        if(temp>maxFitness) {
            maxFitness=temp;
            maxIdx=idx;
        }
        if(temp<minFitness) {
            minFitness=temp;
            minIdx=idx;
        }
    }
    if(eliteIdx==maxIdx)
        failTimes++;
    else
        failTimes=0;
    eliteIdx=maxIdx;
    population[minIdx]=population[maxIdx];
}

void LossyCompressor::crossover() {
    std::vector<ushort> crossoverQueue;
    crossoverQueue.clear();
    for(ushort i=0;i<popSize;i++) {
        if(i==eliteIdx)
            continue;
        if(randD()<crossoverProb)
            crossoverQueue.push_back(i);
    }
    std::random_shuffle(crossoverQueue.begin(),crossoverQueue.end());
    if(crossoverQueue.size()%2==1)
        crossoverQueue.pop_back();

    for(ushort i=0;i<crossoverQueue.size()/2;i++) {
        ushort a=crossoverQueue[2*i];
        ushort b=crossoverQueue[2*i+1];
        // 1<=idx<=length-2
        ushort idx=int(randD()*65535)%(source.size()-2)+1;

        gene::crossover(&population[a],
                        &population[b],idx);
    }
}

void LossyCompressor::mutate() {
    for(ushort i=0;i<popSize;i++) {
        if(i==eliteIdx)
            continue;
        if(randD()<mutateProb) {
            ushort idx=int(randD()*65535)%(source.size());
            population[i].mutate(idx);
        }
    }
}

void LossyCompressor::runGenetic() {
    initialize();
    failTimes=0;
    generation=0;
    emit progressRangeSet(0,0,maxGeneration);
    while(true) {
        emit keepAwake();
        caculateFitness();
        select();

        if(population[eliteIdx].getFitness()>0&&failTimes>=maxFailTimes) {
            std::cerr<<"迭代成功，自然结束"<<std::endl;
            break;
        }
        if(generation>=maxGeneration) {
            if(population[eliteIdx].getFitness()>0)
                std::cerr<<"迭代成功,代数过高，结束"<<std::endl;
            else
                std::cerr<<"迭代失败"<<std::endl;
            break;
        }

        crossover();
        mutate();
        generation++;
        emit progressAdd(1);
    }
    emit progressRangeSet(0,maxGeneration,maxGeneration);
}
