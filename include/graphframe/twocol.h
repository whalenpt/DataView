// twocol.h
#pragma once

#include <QWidget>
#include <QList>
#include <QStringList>
#include <vector>
#include <ParamBin/parambin.hpp>
#include <pwutils/pwdefs.h>
#include "dropwidget.h"
#include "graphframe/axesaux.h"

class GraphFrame;
class QChart;
class QLineSeries;
class QValueAxis;
class QLogValueAxis;

class TwoCol : public DropWidget
{
    Q_OBJECT

public:
    explicit TwoCol(GraphFrame* parent_frame);
    void graph(const QStringList& fnames,pw::FileSignature fsig,\
            pw::DataSignature datasig, pw::OperatorSignature opsig);
    void graph(const QString& fname,pw::FileSignature fsig,\
            pw::DataSignature datasig, pw::OperatorSignature opsig);
    void setAxes(AxesType axes_type);

private:
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


