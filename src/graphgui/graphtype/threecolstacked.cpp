
#include "graphgui/graphframe.h"
#include "graphgui/dropchartview.h"
#include "graphgui/graphtype/threecolstacked.h"
#include "core/fileaux.h"
#include "core/dataaux.h"

#include <QWidget>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QString>
#include <QVBoxLayout>
#include <vector>
#include <pwutils/pwmath.hpp>
#include <pwutils/read/readdat.h>
#include <pwutils/read/readjson.h>
#include <ParamBin/parambin.hpp>

ThreeColStacked::ThreeColStacked(GraphFrame* parent_frame) : QWidget(parent_frame),
    m_parent_frame(parent_frame)
{
    setAcceptDrops(true);
    m_three_col_widget = createThreeColWidget();
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(m_three_col_widget);
    setLayout(vbox);
}

QWidget* ThreeColStacked::createThreeColWidget()
{
    QWidget* widget = new QWidget();
    m_three_col_chart1 = new QChart();
    m_three_col_chart1->legend()->show();
    m_three_col_series1 = new QLineSeries();
    m_three_col_chart1->addSeries(m_three_col_series1);
    m_three_col_view1 = new DropChartView(m_three_col_chart1,m_parent_frame);

    m_three_col_series2 = new QLineSeries();
    m_three_col_chart2 = new QChart();
    m_three_col_chart2->legend()->show();
    m_three_col_chart2->addSeries(m_three_col_series2);
    m_three_col_view2 = new DropChartView(m_three_col_chart2,m_parent_frame);

    m_axisX1 = new QValueAxis;
    m_axisX1->setTickCount(4);
    m_axisX1->setLabelFormat("%.3e");
    m_three_col_chart1->addAxis(m_axisX1,Qt::AlignBottom);
    m_three_col_series1->attachAxis(m_axisX1);

    m_axisX2 = new QValueAxis;
    m_axisX2->setTickCount(4);
    m_axisX2->setLabelFormat("%.3e");
    m_three_col_chart2->addAxis(m_axisX2,Qt::AlignBottom);
    m_three_col_series2->attachAxis(m_axisX2);

    m_axisY1 = new QValueAxis;
    m_axisY1->setTickCount(4);
    m_axisY1->setLabelFormat("%.3e");
    m_three_col_chart1->addAxis(m_axisY1,Qt::AlignLeft);
    m_three_col_series1->attachAxis(m_axisY1);

    m_axisY2 = new QValueAxis;
    m_axisY2->setTickCount(4);
    m_axisY2->setLabelFormat("%.3e");
    m_three_col_chart2->addAxis(m_axisY2,Qt::AlignLeft);
    m_three_col_series2->attachAxis(m_axisY2);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(m_three_col_view1);
    vbox->addWidget(m_three_col_view2);
    widget->setLayout(vbox);
    return widget;
}

void ThreeColStacked::graph(const QString& fname,pw::FileSignature filesig,\
        pw::DataSignature datasig,pw::OperatorSignature opsig)
{
    ParamBin bin = dataaux::XY_CToSeries(fname,*m_three_col_series1,*m_three_col_series2,filesig);
    dataaux::formatAxisX(bin,*m_axisX1);
    bin.set("min_yval",bin.getDbl("min_yval1"));
    bin.set("max_yval",bin.getDbl("max_yval1"));
    dataaux::formatAxisY(bin,*m_axisY1);

    dataaux::formatAxisX(bin,*m_axisX2);
    bin.set("min_yval",bin.getDbl("min_yval2"));
    bin.set("max_yval",bin.getDbl("max_yval2"));
    dataaux::formatAxisY(bin,*m_axisY2);

    m_axisY1->setTickCount(6);
    m_axisY1->setLabelFormat("%.3e");
    m_axisY2->setTickCount(6);
    m_axisY2->setLabelFormat("%.3e");
}



