#include "twocol.h"
#include "dataaux.h"
#include "draglistview.h"
#include "dropchartview.h"
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <QMimeData>
#include <QVBoxLayout>
#include <QDebug>
#include <vector>
#include <parambin.hpp>

TwoCol::TwoCol(GraphFrame* c_parent_frame) : QWidget(c_parent_frame),
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

QtCharts::QChart* TwoCol::createTwoColChart()
{
    series = new QtCharts::QLineSeries();
    chart = new QtCharts::QChart();
    chart->legend()->setVisible(true);
    chart->addSeries(series);
    chart->createDefaultAxes();

    axisX = new QtCharts::QValueAxis;
    axisX->setTickCount(4);
    axisX->setLabelFormat("%.1e");
    chart->setAxisX(axisX,series);

    axisY = new QtCharts::QValueAxis;
    axisY->setTickCount(4);
    axisY->setLabelFormat("%.1e");
    chart->setAxisY(axisY,series);
    return chart;
}

void TwoCol::graph(const QStringList& fnames){
    if(fnames.length()>1){
        qDebug() << QString("TwoCol::graph does not handle multiple file names");
    }
    else
        graph(fnames[0].toStdString());
}

void TwoCol::graph(const std::string& fname)
{
    ParamBin bin;
    dataaux::twoColFileToSeries(fname,*series,bin);
    dataaux::formatAxisX(bin,*axisX);
    dataaux::formatAxisY(bin,*axisY);
    dataaux::setSeriesName(fname,*series);
}

