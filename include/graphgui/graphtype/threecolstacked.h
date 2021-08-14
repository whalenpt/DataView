#ifndef THREECOLSTACKED_H
#define THREECOLSTACKED_H

#include <QWidget>
#include <QStringList>
#include <vector>
#include <string>
#include <ParamBin/parambin.hpp>
#include <pwutils/pwdefs.h>

class GraphFrame;
class DropChartView;
class QLineSeries;
class QChart;
class QValueAxis;

class ThreeColStacked : public QWidget
{
    Q_OBJECT

public:
    explicit ThreeColStacked(GraphFrame* parent_frame);
    void graph(const QString& fname,pw::FileSignature filesig,\
            pw::DataSignature datasig,\
            pw::OperatorSignature opsig);

private:
    GraphFrame* m_parent_frame;
    QWidget* createThreeColWidget();
    QWidget* m_three_col_widget;

    QLineSeries* m_three_col_series1;
    QLineSeries* m_three_col_series2;
    DropChartView* m_three_col_view1;
    DropChartView* m_three_col_view2;
    QChart* m_three_col_chart1;
    QChart* m_three_col_chart2;
    QValueAxis* m_axisX1;
    QValueAxis* m_axisX2;
    QValueAxis* m_axisY1;
    QValueAxis* m_axisY2;
};

#endif // THREECOLSTACKEDGRAPHS_H
