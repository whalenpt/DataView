
#include "graphgui/graphframe.h"
#include "graphgui/dropchartview.h"
#include "graphgui/graphtype/threecolstacked.h"
#include "core/fileaux.h"

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
    m_three_col_chart1->legend()->hide();
    m_three_col_series1 = new QLineSeries();
    m_three_col_chart1->addSeries(m_three_col_series1);

    m_three_col_view1 = new DropChartView(m_three_col_chart1);
    m_three_col_view1->setRenderHint(QPainter::Antialiasing);
    connect(m_three_col_view1,SIGNAL(fileDrop(const QString&)),
        m_parent_frame,SLOT(graphFile(const QString&)));

    m_three_col_series2 = new QLineSeries();
    m_three_col_chart2 = new QChart();
    m_three_col_chart2->legend()->hide();
    m_three_col_chart2->addSeries(m_three_col_series2);
    m_three_col_view2 = new DropChartView(m_three_col_chart2);
    m_three_col_view2->setRenderHint(QPainter::Antialiasing);
    connect(m_three_col_view2,SIGNAL(fileDrop(const QString&)),
        m_parent_frame,SLOT(graphFile(const QString&)));

    m_axisX1 = new QValueAxis;
    m_axisX1->setTickCount(4);
    m_axisX1->setLabelFormat("%.1e");
    m_three_col_chart1->addAxis(m_axisX1,Qt::AlignBottom);

    m_axisX2 = new QValueAxis;
    m_axisX2->setTickCount(4);
    m_axisX2->setLabelFormat("%.1e");
    m_three_col_chart2->addAxis(m_axisX2,Qt::AlignBottom);

    m_axisY1 = new QValueAxis;
    m_axisY1->setTickCount(4);
    m_axisY1->setLabelFormat("%.1e");
    m_three_col_chart1->addAxis(m_axisY1,Qt::AlignLeft);

    m_axisY2 = new QValueAxis;
    m_axisY2->setTickCount(4);
    m_axisY2->setLabelFormat("%.1e");
    m_three_col_chart2->addAxis(m_axisY2,Qt::AlignLeft);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(m_three_col_view1);
    vbox->addWidget(m_three_col_view2);
    widget->setLayout(vbox);
    return widget;
}

void ThreeColStacked::graph(const QString& fname,pw::FileSignature filesig,\
        pw::DataSignature datasig,pw::OperatorSignature opsig)
{
    m_three_col_series1->clear();
    m_three_col_series2->clear();
    std::vector<double> x;
    std::vector<double> y1;
    std::vector<double> y2;
    ParamBin bin;
    if(filesig == pw::FileSignature::DAT)
        bin = ParamBin({dat::readXY_C(fname.toStdString(),x,y1,y2)});
    else if(filesig == pw::FileSignature::JSON)
        bin = ParamBin({json::readXY_C(fname.toStdString(),x,y1,y2)});

    double min_xval = pw::min(x);
    double max_xval = pw::max(x);
    double min_yval1 = pw::min(y1);
    double max_yval1 = pw::max(y1);
    double min_yval2 = pw::min(y2);
    double max_yval2 = pw::max(y2);
    // Workaround QValueAxis setRange issue handling small numbers
    if(fabs(min_xval) < 1.0e-12 || fabs(max_xval) < 1.0e-12){
       double mult_fact = 1.0e15;
       min_xval *= mult_fact;
       max_xval *= mult_fact;
       for(unsigned int i = 0; i < x.size(); i++){
           m_three_col_series1->append(mult_fact*x[i],y1[i]);
           m_three_col_series2->append(mult_fact*x[i],y2[i]);
       }
    } else{
        for(unsigned int i = 0; i < x.size(); i++){
           m_three_col_series1->append(x[i],y1[i]);
           m_three_col_series2->append(x[i],y2[i]);
        }
    }
    m_axisX1->setRange(min_xval,max_xval);
    m_axisX2->setRange(min_xval,max_xval);
    if(bin.inBin("xlabel")){
        std::string xlabel = bin.getStr("xlabel");
        if(bin.inBin("xunit_str")){
            xlabel = xlabel + " (" + bin.getStr("xunit_str") + ")";
        }
        m_axisX1->setTitleText(QString::fromStdString(xlabel));
        m_axisX2->setTitleText(QString::fromStdString(xlabel));
    }
    m_axisY1->setRange(min_yval1,max_yval1);
    m_axisY2->setRange(min_yval2,max_yval2);
}



