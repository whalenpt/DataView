
#include "graphframe/surfacewidget.h"
#include "graphframe/graphframe.h"
#include "dropwidget.h"
#include "draglistview.h"
#include "core/dataaux3D.h"


#include <QChart>
#include <QLineSeries>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <Q3DSurface>
#include <QSurfaceDataProxy>
#include <QSurface3DSeries>
#include <QValue3DAxis>
#include <Q3DTheme>
#include <Q3DCamera>
#include <QFont>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QByteArray>
#include <QDataStream>
#include <QWidget>

#include <cmath>
#include <pwutils/pwmath.hpp>
#include <ParamBin/parambin.hpp>


SurfaceWidget::SurfaceWidget(GraphFrame* parent_frame) :
    m_parent_frame(parent_frame)
{
    QHBoxLayout* hbox = new QHBoxLayout();
    QVBoxLayout* vbox = new QVBoxLayout();
    m_surface_graph = new SurfaceGraph(parent_frame);

    hbox->addWidget(m_surface_graph);
    hbox->addLayout(vbox);
    vbox->setAlignment(Qt::AlignTop);
    setLayout(hbox);
}

SurfaceWidget::~SurfaceWidget()
{ }


SurfaceGraph::SurfaceGraph(GraphFrame* parent_frame,QWidget* parent_widget) : 
    QWidget(parent_widget),
    m_parent_frame(parent_frame),
    m_graph(new Q3DSurface()),
    m_maxpoint2DX(200),
    m_maxpoint2DY(200)
{
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);
    m_graph->axisZ()->setTitle("x");
    m_graph->axisX()->setTitle("y");
    m_graph->axisY()->setTitle("z");
    m_graph->axisZ()->setTitleVisible(true);
    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitleVisible(true);
    m_graph->setAspectRatio(2.0);
    m_graph->setHorizontalAspectRatio(1.0);

    //m_graph->setActiveTheme(new Q3DTheme(Q3DTheme::ThemeEbony));
    m_graph->setActiveTheme(new Q3DTheme(Q3DTheme::ThemeStoneMoss));
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(20);
    m_graph->activeTheme()->setFont(font);
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

    QWidget* graph_widget = QWidget::createWindowContainer(m_graph);
    graph_widget->setFocusPolicy(Qt::StrongFocus);
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(graph_widget);
    setLayout(vbox);
}

SurfaceGraph::~SurfaceGraph()
{ }

void SurfaceGraph::graph(const QString& fname,pw::FileSignature fsig,\
        pw::DataSignature datasig, pw::OperatorSignature opsig)
{
    clearSeries();
    ParamBin bin = dataaux3D::readXYZData(fname,m_x,m_y,m_z,fsig,m_maxpoint2DX,m_maxpoint2DY);
    dataaux3D::fillSurfaceDataItems(*m_data_array,m_x,m_y,m_z);
    m_data_proxy->resetArray(m_data_array);
    m_graph->axisX()->setRange(bin.getFloat("min_yval"),bin.getFloat("max_yval"));
    m_graph->axisY()->setRange(bin.getFloat("min_zval"),bin.getFloat("max_zval"));
    m_graph->axisZ()->setRange(bin.getFloat("min_xval"),bin.getFloat("max_xval"));

    if(bin.inBin("xlabel")){
        m_graph->axisZ()->setTitle(QString::fromStdString(bin.getStr("xlabel")));
    } else
        m_graph->axisZ()->setTitle("x");
    if(bin.inBin("ylabel"))
        m_graph->axisX()->setTitle(QString::fromStdString(bin.getStr("ylabel")));
    else
        m_graph->axisX()->setTitle("y");

    if(bin.inBin("zlabel"))
        m_graph->axisY()->setTitle(QString::fromStdString(bin.getStr("zlabel")));
    else
        m_graph->axisY()->setTitle("z");
}


void SurfaceGraph::clearSeries()
{
    // m_data_proxy clears (deletes) m_data_array 
    m_data_proxy->resetArray(nullptr);
    m_x.clear();
    m_y.clear();
    m_z.clear();
    m_data_array = new QSurfaceDataArray;
}








