#ifndef TWOCOL_M_LOG_H
#define TWOCOL_M_LOG_H

#include <QWidget>
#include <QStringList>
#include <vector>
#include <parambin.hpp>

class GraphFrame;
class QChart;
class DropChartView;
class QLineSeries;
class QValueAxis;
class QLogValueAxis;

class TwoColMLog : public QWidget
{
    Q_OBJECT

public:
    explicit TwoColMLog(GraphFrame* c_parent_frame);
    void graph(const QStringList& fnames);

private:
    GraphFrame* parent_frame;
    QChart* createChart();
    ParamBin createLineSeries(const QString& fname,double& minx,double& maxx,double& miny,double& maxy);
    void clearLineSeries();
    DropChartView* view;
    QChart* chart;
    std::vector<QLineSeries*> line_series_vec;
    QValueAxis* axisX;
    QLogValueAxis* axisY;
};
#endif // MLOGTWOCOL_H
