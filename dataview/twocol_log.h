#ifndef TWOCOL_LOG_H
#define TWOCOL_LOG_H

#include <QWidget>
#include <QStringList>
#include <string>

class GraphFrame;
class QChart;
class DropChartView;
class QLineSeries;
class QValueAxis;
class QLogValueAxis;

class TwoColLog : public QWidget
{
    Q_OBJECT

public:
    explicit TwoColLog(GraphFrame* c_parent_frame);
    void graph(const std::string& fname);
    void graph(const QStringList& fnames);

private:
    GraphFrame* parent_frame;
    QChart* createTwoColChart();
    DropChartView* view;
    QChart* chart;
    QLineSeries* series;
    QValueAxis* axisX;
    QLogValueAxis* axisY;
};

#endif // TWOCOL_LOG_H
