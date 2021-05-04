#ifndef TWOCOL_M_H
#define TWOCOL_M_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <vector>
#include <parambin.hpp>

class GraphFrame;
class QChart;
class DropChartView;
class QLineSeries;
class QValueAxis;

class TwoColM : public QWidget
{
    Q_OBJECT

public:
    explicit TwoColM(GraphFrame* c_parent_frame);
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
    QValueAxis* axisY;
};

#endif // MULTITWOCOL_H
