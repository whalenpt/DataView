#ifndef TWOCOL_M_LOG_H
#define TWOCOL_M_LOG_H

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

class TwoColMLog : public QWidget
{
    Q_OBJECT

public:
    explicit TwoColMLog(GraphFrame* c_parent_frame);
    void graph(const QStringList& fnames);

private:
    GraphFrame* parent_frame;
    QtCharts::QChart* createChart();
    ParamBin createLineSeries(const QString& fname,double& minx,double& maxx,double& miny,double& maxy);
    void clearLineSeries();
    DropChartView* view;
    QtCharts::QChart* chart;
    std::vector<QtCharts::QLineSeries*> line_series_vec;
    QtCharts::QValueAxis* axisX;
    QtCharts::QLogValueAxis* axisY;
};
#endif // MLOGTWOCOL_H
