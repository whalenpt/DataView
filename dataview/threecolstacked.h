#ifndef THREECOLSTACKEDGRAPHS_H
#define THREECOLSTACKEDGRAPHS_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QStringList>
#include <vector>
#include <string>
#include <fstream>
#include <parambin.hpp>
#include "dropchartview.h"
#include "graphframe.h"

class GraphFrame;

class ThreeColStacked : public QWidget
{
    Q_OBJECT

public:
    explicit ThreeColStacked(GraphFrame* c_parent_frame);
    void graph(const std::string& fname);
    void graph(const QStringList& fnames);

private:
    GraphFrame* parent_frame;
    ParamBin readThreeColDoubles(const std::string& fname,std::vector<double>& x,std::vector<double>& y1,
                                     std::vector<double>& y2);
    QWidget* createThreeColWidget();
    QWidget* three_col_widget;

    QtCharts::QLineSeries* three_col_series1;
    QtCharts::QLineSeries* three_col_series2;
    DropChartView* three_col_view1;
    DropChartView* three_col_view2;
    QtCharts::QChart* three_col_chart1;
    QtCharts::QChart* three_col_chart2;
    QtCharts::QValueAxis* axisX1;
    QtCharts::QValueAxis* axisX2;
    QtCharts::QValueAxis* axisY1;
    QtCharts::QValueAxis* axisY2;
};

#endif // THREECOLSTACKEDGRAPHS_H
