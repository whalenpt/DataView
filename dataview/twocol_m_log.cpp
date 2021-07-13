#include "twocol_m_log.h"
#include "graphframe.h"
#include "dropchartview.h"
#include "fileaux.h"

#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QStringList>
#include <QString>

#include <QChart>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QLineSeries>

#include <pwutils/pwmath.hpp>
#include <parambin.hpp>

TwoColMLog::TwoColMLog(GraphFrame* c_parent_frame) : QWidget(c_parent_frame),
    parent_frame(c_parent_frame)
{
    setAcceptDrops(true);
    view = new DropChartView(createChart());
    view->setRenderHint(QPainter::Antialiasing);
    connect(view,SIGNAL(fileDrop(const QStringList&)),
            parent_frame,SLOT(graph(const QStringList&)));
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(view);
    setLayout(vbox);
}

QChart* TwoColMLog::createChart()
{
    chart = new QChart();
    chart->legend()->setVisible(true);

    axisX = new QValueAxis;
    axisX->setTickCount(4);
    axisX->setLabelFormat("%.1e");
    chart->addAxis(axisX,Qt::AlignBottom);

    axisY = new QLogValueAxis;
    axisY->setLabelFormat("%.1e");
    chart->addAxis(axisY,Qt::AlignLeft);

    return chart;
}

ParamBin TwoColMLog::createLineSeries(const QString& fname,
                         double& minx,double& maxx,double& miny,double& maxy)
{
    QLineSeries* series = new QLineSeries();
    series->setName(fileaux::getLocalFileName(fname));
    std::vector<double> x;
    std::vector<double> y;
    ParamBin bin = fileaux::readTwoColDoubles(fname.toStdString(),x,y);
    minx = x[0];
    maxx = x[x.size()-1];
    miny = pw::min(y);
    maxy = pw::max(y);

    // Workaround QValueAxis setRange issue handling small numbers
    if(fabs(minx) < 1.0e-12 || fabs(maxx) < 1.0e-12){
       double mult_fact = 1.0e15;
       minx *= mult_fact;
       maxx *= mult_fact;
       for(unsigned int i = 0; i < x.size(); i++)
           series->append(mult_fact*x[i],y[i]);
    } else{
        for(unsigned int i = 0; i < x.size(); i++)
           series->append(x[i],y[i]);
    }

    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    line_series_vec.push_back(series);
    return bin;
}

void TwoColMLog::clearLineSeries()
{
    for(unsigned int i = 0; i < line_series_vec.size();i++){
        chart->removeSeries(line_series_vec[i]);
        delete line_series_vec[i];
    }
    line_series_vec.clear();
}

void TwoColMLog::graph(const QStringList& fnames){

    clearLineSeries();
    std::vector<double> min_xval_vec;
    std::vector<double> max_xval_vec;
    std::vector<double> min_yval_vec;
    std::vector<double> max_yval_vec;

    ParamBin bin;
    for(int i = 0; i < fnames.size(); i++){
        QString fname = fnames[i];
        double minx,maxx,miny,maxy;
        bin = createLineSeries(fname,minx,maxx,miny,maxy);
        min_xval_vec.push_back(minx);
        max_xval_vec.push_back(maxx);
        min_yval_vec.push_back(miny);
        max_yval_vec.push_back(maxy);
    }
    double min_xval = pw::min(min_xval_vec);
    double max_xval = pw::max(max_xval_vec);
    double min_yval = pw::min(min_yval_vec);
    double max_yval = pw::max(max_yval_vec);

    axisX->setRange(min_xval,max_xval);
    if(bin.inBin("xlabel")){
        std::string xlabel = bin.getStr("xlabel");
        if(bin.inBin("xunit_str")){
            xlabel = xlabel + " (" + bin.getStr("xunit_str") + ")";
        }
        axisX->setTitleText(QString::fromStdString(xlabel));
    }
    axisY->setRange(min_yval,max_yval);
    if(bin.inBin("ylabel")){
        std::string ylabel = bin.getStr("ylabel");
        if(bin.inBin("yunit_str")){
            ylabel = ylabel + " (" + bin.getStr("yunit_str") + ")";
        }
        axisY->setTitleText(QString::fromStdString(ylabel));
    }
}



