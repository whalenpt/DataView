
#include "graphframe/surfacegraph.h"
#include "graphframe/graphframe.h"
#include "graphframe/dropchartview.h"
#include "core/dataaux3D.h"

#include <QChart>
#include <QLineSeries>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <pwutils/pwmath.hpp>
#include <ParamBin/parambin.hpp>
#include <Q3DSurface>
#include <QSurfaceDataProxy>
#include <QSurface3DSeries>
#include <QValue3DAxis>
#include <Q3DTheme>
#include <Q3DCamera>
#include <QFont>
#include <cmath>

SurfaceGraph::SurfaceGraph(GraphFrame* parent_frame) : QWidget(parent_frame),
    m_parent_frame(parent_frame)
{
    setAcceptDrops(true);
    m_graph = new Q3DSurface();
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);
    m_graph->setActiveTheme(new Q3DTheme(Q3DTheme::ThemeEbony));
    m_graph->activeTheme()->setFont(QFont("Times New Roman",24));
    m_graph->axisX()->setLabelFormat("%.2e");
    m_graph->axisY()->setLabelFormat("%.2e");
    m_graph->axisZ()->setLabelFormat("%.2e");
    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);


    m_data_proxy = new QSurfaceDataProxy();
    m_series = new QSurface3DSeries(m_data_proxy);
    m_series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_series->setFlatShadingEnabled(true);
    m_series->setItemLabelFormat(QStringLiteral("(@xLabel,@zLabel): @yLabel"));

    QLinearGradient grad;
    grad.setColorAt(0.0, Qt::darkGreen);
    grad.setColorAt(0.5, Qt::yellow);
    grad.setColorAt(0.8, Qt::red);
    grad.setColorAt(1.0, Qt::darkRed);
    m_series->setBaseGradient(grad);
    m_series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    m_graph->addSeries(m_series);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricLeft);

    m_data_array = new QSurfaceDataArray;
    QVBoxLayout* vbox = new QVBoxLayout();
    QWidget* graph_widget = QWidget::createWindowContainer(m_graph);
    graph_widget->setFocusPolicy(Qt::StrongFocus);
    vbox->addWidget(graph_widget);
    setLayout(vbox);
}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::graph(const QString& fname,pw::FileSignature fsig,\
        pw::DataSignature datasig, pw::OperatorSignature opsig)
{
    clearSeries();
    ParamBin bin = dataaux3D::XYZToSurfaceDataArray(fname,*m_data_array,fsig);
    m_data_proxy->resetArray(m_data_array);
    m_graph->axisX()->setRange(bin.getDbl("min_yval"),bin.getDbl("max_yval"));
    m_graph->axisY()->setRange(bin.getDbl("min_zval"),bin.getDbl("max_zval"));
    m_graph->axisZ()->setRange(bin.getDbl("min_xval"),bin.getDbl("max_xval"));
    if(bin.inBin("xlabel")){
        m_graph->axisZ()->setTitle(QString::fromStdString(bin.getStr("xlabel")));
        m_graph->axisZ()->setTitleVisible(true);
    }
    if(bin.inBin("ylabel")){
        m_graph->axisX()->setTitle(QString::fromStdString(bin.getStr("ylabel")));
        m_graph->axisX()->setTitleVisible(true);
    }
    if(bin.inBin("zlabel")){
        m_graph->axisY()->setTitle(QString::fromStdString(bin.getStr("zlabel")));
        m_graph->axisY()->setTitleVisible(true);
    }
}

void SurfaceGraph::clearSeries()
{
    // m_data_proxy clears (deletes) m_data_array 
    m_data_proxy->resetArray(nullptr);
    m_data_array = new QSurfaceDataArray;
}







