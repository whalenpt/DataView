#include "twocol_m.h"
#include "fileaux.h"
#include "dropchartview.h"
#include "graphframe.h"

#include <QWidget>
#include <QPainter>
#include <QChart>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QLineSeries>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
//#include <QDebug>
#include <pwutils/pwmath.hpp>
#include <ParamBin/parambin.hpp>

TwoColM::TwoColM(GraphFrame* parent_frame) : QWidget(parent_frame),
    m_parent_frame(parent_frame)
{
    setAcceptDrops(true);

    m_chart = new QChart();
    m_axisX = new QValueAxis;
    m_axisY = new QValueAxis;
    m_axes.append(m_axisX);
    m_axes.append(m_axisY);
    m_axislogX = new QLogValueAxis;
    m_axislogY = new QLogValueAxis;
    m_logaxes.append(m_axislogX);
    m_logaxes.append(m_axislogY);

    initChart();
    initAxes();
    setAxes(AxesType::Standard);

    m_view = new DropChartView(m_chart);
    m_view->setRenderHint(QPainter::Antialiasing);
    connect(m_view,SIGNAL(fileDrop(const QStringList&)),
            m_parent_frame,SLOT(graph(const QStringList&)));
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(m_view);
    setLayout(vbox);
}

void TwoColM::setAxes(AxesType axes_type) {
    if(axes_type == m_axes_type)
        return;
    m_axes_type = axes_type;
    axesaux::setAxes(axes_type,m_chart,m_line_series_vec,m_axes,m_logaxes);
}

void TwoColM::initChart()
{
    m_chart->legend()->setVisible(true);
}

void TwoColM::initAxes()
{
    m_axes_type = AxesType::Standard;

    m_axisX->setTickCount(4);
    m_axisX->setLabelFormat("%.1e");
    m_chart->addAxis(m_axisX,Qt::AlignBottom);

    m_axisY->setTickCount(4);
    m_axisY->setLabelFormat("%.1e");
    m_chart->addAxis(m_axisY,Qt::AlignLeft);

    m_axislogX->setLabelFormat("%.1e");
    m_axislogY->setLabelFormat("%.1e");
}

void TwoColM::formatAxes(const ParamBin& bin)
{
    if(m_axes_type == AxesType::Standard){
        dataaux::formatAxisX(bin,*m_axisX);
        dataaux::formatAxisY(bin,*m_axisY);
    } else if(m_axes_type == AxesType::Semilogy) {
        dataaux::formatAxisX(bin,*m_axisX);
        dataaux::formatAxisLogY(bin,*m_axislogY);
    } else if(m_axes_type == AxesType::Semilogx) {
        dataaux::formatAxisLogX(bin,*m_axislogX);
        dataaux::formatAxisY(bin,*m_axisY);
    } else if(m_axes_type == AxesType::Loglog){
        dataaux::formatAxisLogX(bin,*m_axislogX);
        dataaux::formatAxisLogY(bin,*m_axislogY);
    }
}

void TwoColM::graph(const QStringList& fnames,AxesType axes_type){

    ParamBin bin;
    dataaux::twoColFilesToSeries(fnames,m_line_series_vec,bin);
    this->setAxes(axes_type);
    this->formatAxes(bin);

//    //clearLineSeries();
//    std::vector<double> min_xval_vec;
//    std::vector<double> max_xval_vec;
//    std::vector<double> min_yval_vec;
//    std::vector<double> max_yval_vec;
//
//    ParamBin bin;
//    for(int i = 0; i < fnames.size(); i++){
//        QString fname = fnames[i];
//        double minx,maxx,miny,maxy;
//        bin = createLineSeries(fname,minx,maxx,miny,maxy);
//        min_xval_vec.push_back(minx);
//        max_xval_vec.push_back(maxx);
//        min_yval_vec.push_back(miny);
//        max_yval_vec.push_back(maxy);
//    }
//    double min_xval = pw::min(min_xval_vec);
//    double max_xval = pw::max(max_xval_vec);
//    double min_yval = pw::min(min_yval_vec);
//    double max_yval = pw::max(max_yval_vec);
//
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


