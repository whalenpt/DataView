// surfacegraph.h
#pragma once

#include <QWidget>
#include <QList>
#include <QStringList>
#include <QSurfaceDataItem>
#include <vector>
#include <pwutils/pwdefs.h>

class GraphFrame;
class QChart;
class DropChartView;
class Q3DSurface;
class QSurface3DSeries;
class QSurfaceDataProxy;

class SurfaceGraph : public QWidget
{
    Q_OBJECT

public:
    explicit SurfaceGraph(GraphFrame* parent_frame);
    ~SurfaceGraph();
    void graph(const QString& fname,pw::FileSignature fsig,\
            pw::DataSignature datasig, pw::OperatorSignature opsig);

private:
    GraphFrame* m_parent_frame;
    DropChartView* m_view;
    QChart* m_chart;
    Q3DSurface* m_graph;
    QSurface3DSeries* m_series;
    QSurfaceDataProxy* m_data_proxy;
    //QSurfaceDataArray is a typedef for QList<QList<QSurfaceDataItem>*>
    QList<QList<QSurfaceDataItem>*>* m_data_array;

    void clearSeries();
};


