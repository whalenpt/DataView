
#include "graphgui/graphframe.h"
#include "graphgui/dropchartview.h"
#include "graphgui/axesaux.h"
#include "graphgui/graphtype/twocol.h"
#include "core/dataaux.h"

#include <QWidget>
#include <QPainter>
#include <QChart>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QLineSeries>
#include <QAbstractSeries>
#include <QString>
#include <QVBoxLayout>
#include <vector>
#include <ParamBin/parambin.hpp>

//#include <QDebug>

TwoCol::TwoCol(GraphFrame* parent_frame) : QWidget(parent_frame),
    m_parent_frame(parent_frame), m_axes_type(AxesType::Standard)
{
    setAcceptDrops(true);

    m_series = new QLineSeries();
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

void TwoCol::setAxes(AxesType axes_type) {
    if(axes_type == m_axes_type)
        return;
    m_axes_type = axes_type;
    axesaux::setAxes(axes_type,m_chart,m_series,m_axes,m_logaxes);
}

void TwoCol::initChart()
{
    m_chart->legend()->setVisible(true);
    m_chart->addSeries(m_series);
}

void TwoCol::initAxes()
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

    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);
}

void TwoCol::graph(const QStringList& fnames,AxesType axes_type){
    if(fnames.length()>1)
        qDebug() << QString("TwoCol::graph does not handle multiple file names");
    else
        graph(fnames[0].toStdString(),axes_type);
}

void TwoCol::formatAxes(const ParamBin& bin)
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

void TwoCol::graph(const std::string& fname,AxesType axes_type)
{
    ParamBin bin;
    dataaux::twoColFileToSeries(fname,*m_series,bin);
    this->setAxes(axes_type);
    this->formatAxes(bin);
}

