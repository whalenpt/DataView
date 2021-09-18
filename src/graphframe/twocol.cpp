

#include "dropwidget.h"
#include "graphframe/twocol.h"
#include "graphframe/graphframe.h"
#include "graphframe/axesaux.h"
#include "core/dataaux.h"

#include <QWidget>
#include <QPainter>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QLineSeries>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <pwutils/pwmath.hpp>
#include <ParamBin/parambin.hpp>

TwoCol::TwoCol(GraphFrame* parent_frame) : 
    DropWidget(parent_frame),
    m_parent_frame(parent_frame)
{
    m_chart = new QChart();
    m_chart->legend()->setVisible(true);
    m_view = new DropChartView(m_chart,parent_frame);
    connect(m_view,&DropChartView::fileDrop,parent_frame,&GraphFrame::graphFiles);

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
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(m_view);
    setLayout(vbox);

    connect(this,&DropWidget::fileDrop,
            parent_frame,&GraphFrame::graphFiles);
}

void TwoCol::setAxes(AxesType axes_type) {
    m_axes_type = axes_type;
    axesaux::setAxes(axes_type,m_chart,m_line_series_vec,m_axes,m_logaxes);
}

void TwoCol::initAxes()
{
    m_axes_type = AxesType::Standard;
    m_axisX->setTickCount(4);
    m_axisX->setLabelFormat("%.2e");
    m_chart->addAxis(m_axisX,Qt::AlignBottom);

    m_axisY->setTickCount(4);
    m_axisY->setLabelFormat("%.2e");
    m_chart->addAxis(m_axisY,Qt::AlignLeft);

    m_axislogX->setLabelFormat("%.2e");
    m_axislogY->setLabelFormat("%.2e");
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

void TwoCol::clearLineSeries(){
    for(auto series : m_chart->series())
        m_chart->removeSeries(series);
    for(auto series : m_line_series_vec)
        delete series;
    m_line_series_vec.clear();
}

void TwoCol::graph(const QString& fname,pw::FileSignature fsig,\
        pw::DataSignature datasig, pw::OperatorSignature opsig)
{
    graph(QStringList({fname}), fsig,datasig,opsig);
}

//void TwoColM::graph(const QStringList& fnames,AxesType axes_type){
void TwoCol::graph(const QStringList& fnames,pw::FileSignature fsig,\
        pw::DataSignature datasig, pw::OperatorSignature opsig)
{
    clearLineSeries();
    ParamBin bin = dataaux::multiXYToSeries(fnames,m_line_series_vec,fsig);
    for(auto series : m_line_series_vec)
        m_chart->addSeries(series);

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


