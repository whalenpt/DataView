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
class QSlider;

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
    void setRangeX(float minX,float maxX);
    void setRangeY(float minY,float maxY);
    float minX() const;
    float maxX() const;
    float minY() const;
    float maxY() const;

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
            pw::DataSignature datasig, pw::OperatorSignature opsig);

private:
    GraphFrame* m_parent_frame;
    SurfaceGraph* m_surface_graph;
    QSlider* m_axisMinSliderX;
    QSlider* m_axisMaxSliderX;
    QSlider* m_axisMinSliderY;
    QSlider* m_axisMaxSliderY;

    void changeMinX(int min);
    void changeMaxX(int max);
    void changeMinY(int min);
    void changeMaxY(int max);

};




