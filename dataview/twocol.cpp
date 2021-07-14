#include "twocol.h"
#include "graphframe.h"
#include "dataaux.h"
#include "dropchartview.h"

#include <QWidget>
#include <QPainter>
#include <QChart>
#include <QValueAxis>
#include <QLineSeries>
#include <QAbstractSeries>
#include <QString>
#include <QVBoxLayout>
#include <vector>
#include <ParamBin/parambin.hpp>

//#include <QDebug>

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

QChart* TwoCol::createTwoColChart()
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

    axisY = new QValueAxis;
    axisY->setTickCount(4);
    axisY->setLabelFormat("%.1e");
    chart->addAxis(axisY,Qt::AlignLeft);
    series->attachAxis(axisY);
    return chart;
}

void TwoCol::graph(const QStringList& fnames){
    if(fnames.length()>1)
        qDebug() << QString("TwoCol::graph does not handle multiple file names");
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

