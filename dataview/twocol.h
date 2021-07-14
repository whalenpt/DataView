#ifndef TWOCOL_H
#define TWOCOL_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QStringList>
#include <vector>
#include <string>
#include <fstream>
#include <ParamBin/parambin.hpp>

class GraphFrame;
class QChart;
class DropChartView;
class QLineSeries;
class QValueAxis;
class QLogValueAxis;

enum class AxesType {Standard,Semilogy,Semilogx,Loglog};

class TwoCol : public QWidget
{
    Q_OBJECT

public:
    explicit TwoCol(GraphFrame* parent_frame);
    void graph(const std::string& fname,AxesType axes_type = AxesType::Standard);
    void graph(const QStringList& fnames,AxesType axes_type = AxesType::Standard);
    void setAxes(AxesType axes_type);

private:
    void initChart();
    void initAxes();
    void formatAxes(const ParamBin& bin);
    GraphFrame* m_parent_frame;
    DropChartView* m_view;
    QChart* m_chart;
    QLineSeries* m_series;
    QValueAxis* m_axisX;
    QValueAxis* m_axisY;
    QLogValueAxis* m_axislogX;
    QLogValueAxis* m_axislogY;
    AxesType m_axes_type;
};

#endif // TWOCOL_H

