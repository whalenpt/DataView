#ifndef TWOCOL_H
#define TWOCOL_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QStringList>
#include <vector>
#include <string>
#include <fstream>
#include <parambin.hpp>

class GraphFrame;
class QChart;
class DropChartView;
class QLineSeries;
class QValueAxis;

class TwoCol : public QWidget
{
    Q_OBJECT

public:
    explicit TwoCol(GraphFrame* c_parent_frame);
    void graph(const std::string& fname);
    void graph(const QStringList& fnames);

private:
    GraphFrame* parent_frame;
    QChart* createTwoColChart();

    DropChartView* view;
    QChart* chart;
    QLineSeries* series;
    QValueAxis* axisX;
    QValueAxis* axisY;
};

#endif // TWOCOL_H

