
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
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QValue3DAxis>
#include <cmath>

SurfaceGraph::SurfaceGraph(GraphFrame* parent_frame) : QWidget(parent_frame),
    m_parent_frame(parent_frame)
{
    setAcceptDrops(true);
    m_graph = new Q3DSurface();
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    m_data_proxy = new QSurfaceDataProxy();
    m_series = new QSurface3DSeries(m_data_proxy);

    const unsigned int rows = 50;
    const unsigned int cols = 50;
    const double xmin = -5.0;
    const double xmax = 5.0;
    const double ymin = -5.0;
    const double ymax = 5.0;

    m_data_array = new QSurfaceDataArray;
    m_data_array->reserve(rows);
    for(auto i = 0; i < rows; i++){
        QSurfaceDataRow* data_row = new QSurfaceDataRow(cols);
        double x = xmin + (xmax-xmin)*i/rows;
        for(auto j = 0; j < cols; j++){
            double y = ymin + (ymax-ymin)*i/cols;
            double z = exp(-(pow(x,2)+pow(y,2)));
            (*data_row)[j].setPosition(QVector3D(x,y,z));
        }
        *m_data_array << data_row;
    }
    m_data_proxy->resetArray(m_data_array);


    m_graph->axisX()->setLabelFormat("%.2f");
    m_graph->axisY()->setLabelFormat("%.2f");
    m_graph->axisZ()->setLabelFormat("%.2f");
    m_graph->axisX()->setRange(xmin,xmax);
    m_graph->axisY()->setRange(ymin,ymax);
    m_graph->addSeries(m_series);
       
    QVBoxLayout* vbox = new QVBoxLayout();
    QWidget* graph_widget = QWidget::createWindowContainer(m_graph);
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
//    graph(QStringList({fname}), fsig,datasig,opsig);
}

void SurfaceGraph::clearSeries()
{
    // m_data_proxy clears (deletes) m_data_array 
    m_data_proxy->resetArray(nullptr);
    m_data_array = new QSurfaceDataArray;
}

//void SurfaceGraphM::graph(const QStringList& fnames,AxesType axes_type){
//void SurfaceGraph::graph(const QStringList& fnames,pw::FileSignature fsig,\
//        pw::DataSignature datasig, pw::OperatorSignature opsig)
//{
//    ParamBin bin = dataaux::multiXYToSeries(fnames,m_line_series_vec,fsig);
//    for(auto series : m_line_series_vec)
//        m_chart->addSeries(series);
//
//    if(opsig == pw::OperatorSignature::NONE)
//        this->setAxes(AxesType::Standard);
//    else if(opsig == pw::OperatorSignature::LOGY)
//        this->setAxes(AxesType::Semilogy);
//    else if(opsig == pw::OperatorSignature::LOGXLOGY)
//        this->setAxes(AxesType::Loglog);
//    else if(opsig == pw::OperatorSignature::LOGX)
//        this->setAxes(AxesType::Semilogx);
//
//    this->formatAxes(bin);
//}







