#ifndef TWOCOLGRAPH_H
#define TWOCOLGRAPH_H

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

class TwoCol : public QWidget
{
    Q_OBJECT

public:
    explicit TwoCol(GraphFrame* c_parent_frame);
    void graph(const std::string& fname);
    void graph(const QStringList& fnames);

private:
    GraphFrame* parent_frame;
    QtCharts::QChart* createTwoColChart();

//    QtCharts::QChartView* view;
    DropChartView* view;
    QtCharts::QChart* chart;
    QtCharts::QLineSeries* series;
    QtCharts::QValueAxis* axisX;
    QtCharts::QValueAxis* axisY;
};

#endif // TWOCOLGRAPH_H

