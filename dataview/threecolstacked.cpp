#include "threecolstacked.h"
#include "graphframe.h"
#include "dropchartview.h"
#include "fileaux.h"

#include <QWidget>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QString>
#include <QVBoxLayout>
//#include <QDebug>
#include <vector>
#include <pwutils/pwmath.hpp>
#include <ParamBin/parambin.hpp>

ThreeColStacked::ThreeColStacked(GraphFrame* c_parent_frame) : QWidget(c_parent_frame),
    parent_frame(c_parent_frame)
{
    setAcceptDrops(true);
    three_col_widget = createThreeColWidget();
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(three_col_widget);
    setLayout(vbox);
}

QWidget* ThreeColStacked::createThreeColWidget()
{
    QWidget* widget = new QWidget();
    three_col_chart1 = new QChart();
    three_col_chart1->legend()->hide();
    three_col_series1 = new QLineSeries();
    three_col_chart1->addSeries(three_col_series1);

    three_col_view1 = new DropChartView(three_col_chart1);
    three_col_view1->setRenderHint(QPainter::Antialiasing);
    connect(three_col_view1,SIGNAL(fileDrop(const QStringList&)),
        parent_frame,SLOT(graph(const QStringList&)));

//    connect(three_col_view1,SIGNAL(fileDrop(const std::string&)),
//        parent_frame,SLOT(graph(const std::string&)));


    three_col_series2 = new QLineSeries();
    three_col_chart2 = new QChart();
    three_col_chart2->legend()->hide();
    three_col_chart2->addSeries(three_col_series2);
    three_col_view2 = new DropChartView(three_col_chart2);
    three_col_view2->setRenderHint(QPainter::Antialiasing);
    connect(three_col_view2,SIGNAL(fileDrop(const QStringList&)),
        parent_frame,SLOT(graph(const QStringList&)));

//    connect(three_col_view2,SIGNAL(fileDrop(const std::string&)),
//        parent_frame,SLOT(graph(const std::string&)));

    axisX1 = new QValueAxis;
    axisX1->setTickCount(4);
    axisX1->setLabelFormat("%.1e");
    three_col_chart1->addAxis(axisX1,Qt::AlignBottom);

    axisX2 = new QValueAxis;
    axisX2->setTickCount(4);
    axisX2->setLabelFormat("%.1e");
    three_col_chart2->addAxis(axisX2,Qt::AlignBottom);

    axisY1 = new QValueAxis;
    axisY1->setTickCount(4);
    axisY1->setLabelFormat("%.1e");
    three_col_chart1->addAxis(axisY1,Qt::AlignLeft);

    axisY2 = new QValueAxis;
    axisY2->setTickCount(4);
    axisY2->setLabelFormat("%.1e");
    three_col_chart2->addAxis(axisY2,Qt::AlignLeft);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(three_col_view1);
    vbox->addWidget(three_col_view2);
    widget->setLayout(vbox);
    return widget;
}

void ThreeColStacked::graph(const QStringList& fnames){
    if(fnames.length()>1){
        qDebug() << QString("ThreeColStacked::graph does not handle multiple file names");
    }
    else
        graph(fnames[0].toStdString());
}

void ThreeColStacked::graph(const std::string& fname){
    three_col_series1->clear();
    three_col_series2->clear();
    std::vector<double> x;
    std::vector<double> y1;
    std::vector<double> y2;
    ParamBin bin = readThreeColDoubles(fname,x,y1,y2);

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
           three_col_series1->append(mult_fact*x[i],y1[i]);
           three_col_series2->append(mult_fact*x[i],y2[i]);
       }
    } else{
        for(unsigned int i = 0; i < x.size(); i++){
           three_col_series1->append(x[i],y1[i]);
           three_col_series2->append(x[i],y2[i]);
        }
    }

    axisX1->setRange(min_xval,max_xval);
    axisX2->setRange(min_xval,max_xval);
    if(bin.inBin("xlabel")){
        std::string xlabel = bin.getStr("xlabel");
        if(bin.inBin("xunit_str")){
            xlabel = xlabel + " (" + bin.getStr("xunit_str") + ")";
        }
        axisX1->setTitleText(QString::fromStdString(xlabel));
        axisX2->setTitleText(QString::fromStdString(xlabel));
    }

    axisY1->setRange(min_yval1,max_yval1);
    axisY2->setRange(min_yval2,max_yval2);
}

ParamBin ThreeColStacked::readThreeColDoubles(const std::string& fname,
                               std::vector<double>& x,std::vector<double>& y1,
                                           std::vector<double>& y2){
    std::ifstream infile;
    fileaux::openFile(fname,infile);
    ParamBin bin = fileaux::getHeaderContent(infile);
    while(!infile.eof()){
        double a,b,c;
        infile >> a >> b >> c;
        x.push_back(a);
        y1.push_back(b);
        y2.push_back(c);
    }
    return bin;
}



