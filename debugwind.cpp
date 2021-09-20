#include "debugwind.h"
#include "ui_debugwind.h"

DebugWind::DebugWind(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DebugWind)
{
    ui->setupUi(this);
    Kernel=new TokiSlopeCraft;
    Compressor=new LossyCompressor;

    connect(Compressor,&LossyCompressor::progressRangeSet,
            this,&DebugWind::progressRangeSet);
    connect(Compressor,&LossyCompressor::progressAdd,
            this,&DebugWind::progressAdd);
    connect(Compressor,&LossyCompressor::keepAwake,
            this,&DebugWind::keepAwake);
}

DebugWind::~DebugWind()
{
    delete Kernel;
    delete Compressor;
    delete ui;
}

void DebugWind::progressRangeSet(int min,int max,int val) {
    ui->progressBar->setRange(min,max);
    ui->progressBar->setValue(val);
}

void DebugWind::progressAdd(int delta) {
    ui->progressBar->setValue(
                ui->progressBar->value()+delta);
}

void DebugWind::keepAwake() {
    QApplication::processEvents();
}

void DebugWind::initialize() {
    {
        QByteArray data[4];
        QString Path="D:/Git/SlopeCraft/Colors/";
        {
            QFile file(Path+"RGB.TokiColor");
            file.open(QFile::OpenModeFlag::ReadOnly);
            data[0]=file.readAll();
            file.close();
        }
        {
            QFile file(Path+"HSV.TokiColor");
            file.open(QFile::OpenModeFlag::ReadOnly);
            data[1]=file.readAll();
            file.close();
        }
        {
            QFile file(Path+"Lab.TokiColor");
            file.open(QFile::OpenModeFlag::ReadOnly);
            data[2]=file.readAll();
            file.close();
        }
        {
            QFile file(Path+"XYZ.TokiColor");
            file.open(QFile::OpenModeFlag::ReadOnly);
            data[3]=file.readAll();
            file.close();
        }
        if(!Kernel->setColorSet(data[0],data[1],data[2],data[3])) {
            qDebug("加载颜色失败");
            return;
        }
    }


    {
        QImage raw;
        raw.load("D:/Git/20_1by1_white.png");
        EImage rawE=QImage2EImage(raw);
        bool allowed[64]={0};
        for (ushort baseColor=1;baseColor<64;baseColor++)
            allowed[baseColor]=true;

        allowed[12]=true;

        simpleBlock block[64]={};
        for(ushort baseColor=0;baseColor<64;baseColor++) {
            block[baseColor].id="minecraft:air";
            block[baseColor].version=0;
            block[baseColor].needGlass=false;
            block[baseColor].doGlow=false;
        }

        if(!Kernel->setType(TokiSlopeCraft::mapTypes::Slope,
                            TokiSlopeCraft::gameVersion::MC17,
                            allowed,block,rawE)) {
            qDebug("设置地图画类型失败");
            return;
        }
    }


    if(!Kernel->convert()) {
        qDebug("convert失败");
        return;
    }

    qDebug("初始化完毕");
    ui->mapCol->setMaximum(Kernel->getImageCols()-1);
/*
    bool setColorSet(const char*,const char*,const char*,const char*);
//can do in colorSetReady:
    bool setType(
        mapTypes,
        gameVersion,
        const bool [64],
        const simpleBlock[64] ,
        const EImage &);

//can do in convertionReady:
    bool convert(convertAlgo=RGB_Better,bool dither=false);
    void getTokiColorPtr(ushort,const TokiColor*[]) const;
*/
}

EImage QImage2EImage(const QImage & qi) {
    EImage ei;
    ei.setZero(qi.height(),qi.width());
    const QRgb * CL=nullptr;
    for(int r=0;r<ei.rows();r++) {
        CL=(const QRgb *)qi.scanLine(r);
        for(int c=0;c<ei.cols();c++)
            ei(r,c)=CL[c];
    }
    return ei;
}

QImage EImage2QImage(const EImage & ei,ushort scale) {
    QImage qi(ei.cols()*scale,ei.rows()*scale,QImage::Format::Format_ARGB32);
    QRgb * CL=nullptr;
    for(int r=0;r<qi.height();r++) {
        CL=(QRgb *)qi.scanLine(r);
        for(int c=0;c<qi.width();c++)
            CL[c]=ei(r/scale,c/scale);
    }
    return qi;
}

void DebugWind::on_BtnLoad_clicked() {
    currentCol.resize(Kernel->getImageRows());
    Kernel->getTokiColorPtr(
                ui->mapCol->value(),
                &currentCol[0]);

    HeightLine HL;
    Eigen::Array<uchar,Eigen::Dynamic,1> temp;
    temp.setZero(currentCol.size());
    HL.make(&currentCol[0],temp);

    QImage tempImg=EImage2QImage(HL.toImg(),3);

    ui->ShowRaw->setPixmap(QPixmap::fromImage(tempImg));
}

