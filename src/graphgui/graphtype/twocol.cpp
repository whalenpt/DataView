
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
    m_axes_type(AxesType::Standard)
{
    setAcceptDrops(true);
    m_chart = new QChart;
    m_chart->legend()->setVisible(true);

    m_series = new QLineSeries();
    m_chart->addSeries(m_series);
    m_view = new DropChartView(m_chart,parent_frame);

    m_axisX = new QValueAxis;
    m_axisY = new QValueAxis;
    m_axes.append(m_axisX);
    m_axes.append(m_axisY);
    m_axislogX = new QLogValueAxis;
    m_axislogY = new QLogValueAxis;
    m_logaxes.append(m_axislogX);
    m_logaxes.append(m_axislogY);
    initAxes();
    setAxes(AxesType::Standard);
    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(m_view);
    setLayout(vbox);
}

void TwoCol::setAxes(AxesType axes_type) {
    if(axes_type == m_axes_type)
        return;
    m_axes_type = axes_type;
    axesaux::setAxes(axes_type,m_chart,m_series,m_axes,m_logaxes);
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

void TwoCol::graph(const QString& fname,pw::FileSignature filesig,
        pw::DataSignature datasig,pw::OperatorSignature opsig)
{
    //dataaux::twoColFileToSeries(fname,*m_series,bin);
    ParamBin bin = dataaux::XYToSeries(fname,*m_series,filesig);
    if(opsig == pw::OperatorSignature::NONE)
        this->setAxes(AxesType::Standard);
    else if(opsig == pw::OperatorSignature::LOGY)
        this->setAxes(AxesType::Semilogy);
    else if(opsig == pw::OperatorSignature::LOGXLOGY)
        this->setAxes(AxesType::Loglog);
    else if(opsig == pw::OperatorSignature::LOGX)
        this->setAxes(AxesType::Semilogx);

    this->formatAxes(bin);
}


