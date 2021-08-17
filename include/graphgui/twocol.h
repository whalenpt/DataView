#ifndef TWOCOL_H
#define TWOCOL_H

#include <QWidget>
#include <QList>
#include <ParamBin/parambin.hpp>
#include <pwutils/pwdefs.h>
#include "graphgui/axesaux.h"

class GraphFrame;
class QChart;
class DropChartView;
class QLineSeries;
class QValueAxis;
class QLogValueAxis;
class DropChartView;


class TwoCol : public QWidget
{
    Q_OBJECT

public:
    explicit TwoCol(GraphFrame* parent_frame);
    void graph(const QString& fname,pw::FileSignature filesig,pw::DataSignature datasig,\
            pw::OperatorSignature opsig);
    void setAxes(AxesType axes_type);

private:
    void initAxes();
    void formatAxes(const ParamBin& bin);
    QChart* m_chart;
    DropChartView* m_view;

    QLineSeries* m_series;
    QValueAxis* m_axisX;
    QValueAxis* m_axisY;
    QLogValueAxis* m_axislogX;
    QLogValueAxis* m_axislogY;
    QList<QValueAxis*> m_axes;
    QList<QLogValueAxis*> m_logaxes;
    AxesType m_axes_type;
};

#endif // TWOCOL_H

