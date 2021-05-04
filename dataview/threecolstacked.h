#ifndef THREECOLSTACKED_H
#define THREECOLSTACKED_H

#include <QWidget>
#include <QStringList>
#include <vector>
#include <string>
#include <parambin.hpp>

class GraphFrame;
class DropChartView;
class QLineSeries;
class QChart;
class QValueAxis;

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

    QLineSeries* three_col_series1;
    QLineSeries* three_col_series2;
    DropChartView* three_col_view1;
    DropChartView* three_col_view2;
    QChart* three_col_chart1;
    QChart* three_col_chart2;
    QValueAxis* axisX1;
    QValueAxis* axisX2;
    QValueAxis* axisY1;
    QValueAxis* axisY2;
};

#endif // THREECOLSTACKEDGRAPHS_H
