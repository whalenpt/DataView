#include "twocol_log.h"
#include "graphframe.h"
#include "dataaux.h"
#include "dropchartview.h"
#include <QWidget>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QString>
#include <QVBoxLayout>
//#include <QDebug>
#include <vector>
#include <pwutils/pwmath.hpp>
#include <ParamBin/parambin.hpp>

TwoColLog::TwoColLog(GraphFrame* c_parent_frame) : QWidget(c_parent_frame),
    parent_frame(c_parent_frame)
{
    setAcceptDrops(true);
    view = new DropChartView(createTwoColChart());
    view->setRenderHint(QPainter::Antialiasing);
    connect(view,SIGNAL(fileDrop(const QStringList&)),
            parent_frame,SLOT(graph(const QStringList&)));
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(view);
    setLayout(vbox);
}

QChart* TwoColLog::createTwoColChart()
{
    series = new QLineSeries();
    chart = new QChart();
    chart->legend()->setVisible(true);
    chart->addSeries(series);

    axisX = new QValueAxis;
    axisX->setTickCount(4);
    axisX->setLabelFormat("%.1e");
    chart->addAxis(axisX,Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QLogValueAxis;
    axisY->setLabelFormat("%.1e");
    chart->addAxis(axisY,Qt::AlignLeft);
    series->attachAxis(axisY);
    return chart;
}

void TwoColLog::graph(const QStringList& fnames){
    graph(fnames[0].toStdString());
//    if(fnames.length()>1){
//        qDebug() << QString("TwoColLog::graph does not handle multiple file names");
//    }
//    else
}

void TwoColLog::graph(const std::string& fname){

    ParamBin bin;
    dataaux::twoColFileToSeries(fname,*series,bin);
    dataaux::formatAxisX(bin,*axisX);
    dataaux::formatAxisLogY(bin,*axisY);
    dataaux::setSeriesName(fname,*series);

//    series->clear();
//    series->setName(fileaux::getLocalFileName(QString::fromStdString(fname)));
//    std::vector<double> x;
//    std::vector<double> y;
//    ParamBin bin = fileaux::readTwoColDoubles(fname,x,y);
//
//    double min_xval = 0.95*pw::min(x);
//    double max_xval = 1.05*pw::max(x);
//    double min_yval = 0.95*pw::min(y);
//    double max_yval = 1.05*pw::max(y);

//    // Workaround QValueAxis setRange issue handling small numbers
//    if(fabs(min_xval) < 1.0e-12 || fabs(max_xval) < 1.0e-12){
//       double mult_fact = 1.0e15;
//       min_xval *= mult_fact;
//       max_xval *= mult_fact;
//       for(unsigned int i = 0; i < x.size(); i++)
//           series->append(mult_fact*x[i],y[i]);
//    } else{
//        for(unsigned int i = 0; i < x.size(); i++)
//           series->append(x[i],y[i]);
//    }

//    axisX->setRange(min_xval,max_xval);
//    if(bin.inBin("xlabel")){
//        std::string xlabel = bin.getStr("xlabel");
//        if(bin.inBin("xunit_str")){
//            xlabel = xlabel + " (" + bin.getStr("xunit_str") + ")";
//        }
//        axisX->setTitleText(QString::fromStdString(xlabel));
//    }
//    axisY->setRange(min_yval,max_yval);
//    if(bin.inBin("ylabel")){
//        std::string ylabel = bin.getStr("ylabel");
//        if(bin.inBin("yunit_str")){
//            ylabel = ylabel + " (" + bin.getStr("yunit_str") + ")";
//        }
//        axisY->setTitleText(QString::fromStdString(ylabel));
//    }
}


