#ifndef TWOCOL_M_H
#define TWOCOL_M_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <vector>
#include <ParamBin/parambin.hpp>
#include <pwutils/pwdefs.h>
#include "graphframe/axesaux.h"

class GraphFrame;
class QChart;
class DropChartView;
class QLineSeries;
class QValueAxis;
class QLogValueAxis;

class TwoColM : public QWidget
{
    Q_OBJECT

public:
    explicit TwoColM(GraphFrame* parent_frame);
    //void graph(const QStringList& fnames,AxesType axes_type = AxesType::Standard);
    void graph(const QStringList& fnames,pw::FileSignature fsig,\
            pw::DataSignature datasig, pw::OperatorSignature opsig);
    void setAxes(AxesType axes_type);

private:
    void initChart();
    void initAxes();
    void formatAxes(const ParamBin& bin);
    void clearLineSeries();

    GraphFrame* m_parent_frame;
    DropChartView* m_view;
    QChart* m_chart;
    std::vector<QLineSeries*> m_line_series_vec;
    QValueAxis* m_axisX;
    QValueAxis* m_axisY;
    QLogValueAxis* m_axislogX;
    QLogValueAxis* m_axislogY;
    QList<QValueAxis*> m_axes;
    QList<QLogValueAxis*> m_logaxes;
    AxesType m_axes_type;
};

#endif // MULTITWOCOL_H
