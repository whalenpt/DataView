#ifndef TWOCOL_LOG_H
#define TWOCOL_LOG_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
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

class TwoColLog : public QWidget
{
    Q_OBJECT

public:
    explicit TwoColLog(GraphFrame* c_parent_frame);
    void graph(const std::string& fname);
    void graph(const QStringList& fnames);

private:
    GraphFrame* parent_frame;
    QtCharts::QChart* createTwoColChart();
    DropChartView* view;
    QtCharts::QChart* chart;
    QtCharts::QLineSeries* series;
    QtCharts::QValueAxis* axisX;
    QtCharts::QLogValueAxis* axisY;
};

#endif // LOGTWOCOL_H
