// surfacewidget.h
#pragma once

#include <QWidget>
#include <QObject>
#include <QList>
#include <QStringList>
#include <QSurfaceDataItem>
#include <vector>
#include <pwutils/pwdefs.h>

class GraphFrame;
class Q3DSurface;
class QSurface3DSeries;
class QSurfaceDataProxy;

class SurfaceGraph : public QWidget
{
    Q_OBJECT

public:
    explicit SurfaceGraph(GraphFrame* parent_frame,QWidget* parent_widget = nullptr);
    ~SurfaceGraph();
    void graph(const QString& fname,pw::FileSignature fsig,\
            pw::DataSignature datasig, pw::OperatorSignature opsig);
    void setMaxPoint2DX(unsigned int points) {m_maxpoint2DX = points;}
    void setMaxPoint2DY(unsigned int points) {m_maxpoint2DY = points;}

private:
    GraphFrame* m_parent_frame;
    Q3DSurface* m_graph;
    QSurface3DSeries* m_series;
    QSurfaceDataProxy* m_data_proxy;
    unsigned int m_maxpoint2DX;
    unsigned int m_maxpoint2DY;
    std::vector<float> m_x;
    std::vector<float> m_y;
    std::vector<float> m_z;

    //QSurfaceDataArray is a typedef for QList<QList<QSurfaceDataItem>*>
    QList<QList<QSurfaceDataItem>*>* m_data_array;

    void clearSeries();
};

class SurfaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SurfaceWidget(GraphFrame* parent_frame);
    ~SurfaceWidget();
    void setMaxPoint2DX(unsigned int points) {m_surface_graph->setMaxPoint2DX(points);}
    void setMaxPoint2DY(unsigned int points) {m_surface_graph->setMaxPoint2DY(points);}
    void graph(const QString& fname,pw::FileSignature fsig,\
            pw::DataSignature datasig, pw::OperatorSignature opsig) {m_surface_graph->graph(fname,fsig,datasig,opsig);}

private:
    GraphFrame* m_parent_frame;
    SurfaceGraph* m_surface_graph;
};


class ListDropWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListDropWidget(QWidget* widget,GraphFrame* parent_frame);
    ~ListDropWidget() {};

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;

signals:
    void fileListDrop(const QStringList& filenames);

};




