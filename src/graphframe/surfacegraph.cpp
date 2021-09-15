
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
#include <QtCore/qmath.h>
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

    m_data_proxy = new QSurfaceDataProxy();
    m_series = new QSurface3DSeries(m_data_proxy);
    m_series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_series->setFlatShadingEnabled(true);

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

void SurfaceGraph::initTest()
{
    const unsigned int rows = 50;
    const unsigned int cols = 50;
    const double xmin = -5.0;
    const double xmax = 5.0;
    const double ymin = -5.0;
    const double ymax = 5.0;
    QSurfaceDataArray* data_array = new QSurfaceDataArray;
    data_array->reserve(rows);
    for(auto i = 0; i < rows; i++){
        QSurfaceDataRow* data_row = new QSurfaceDataRow(cols);
        double x = xmin + (xmax-xmin)*i/static_cast<double>(rows);
        for(auto j = 0; j < cols; j++){
            double y = ymin + (ymax-ymin)*j/static_cast<double>(cols);
            double r2 = pow(x,2)+pow(y,2);
            double z = exp(-r2);
            (*data_row)[j].setPosition(QVector3D(y,z,x));
        }
        *data_array << data_row;
    }
    m_data_proxy->resetArray(data_array);

//    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
//    dataArray->reserve(sampleCountZ);
//    for (int i = 0 ; i < sampleCountZ ; i++) {
//        QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleCountX);
//        // Keep values within range bounds, since just adding step can cause minor drift due
//        // to the rounding errors.
//        float z = qMin(sampleMax, (i * stepZ + sampleMin));
//        int index = 0;
//        for (int j = 0; j < sampleCountX; j++) {
//            float x = qMin(sampleMax, (j * stepX + sampleMin));
//            float R = qSqrt(z * z + x * x) + 0.01f;
//            float y = (qSin(R) / R + 0.24f) * 1.61f;
//            (*newRow)[index++].setPosition(QVector3D(x, y, z));
//        }
//        *dataArray << newRow;
//    }


//    m_data_array->reserve(rows);
//    for(auto i = 0; i < rows; i++){
//        QSurfaceDataRow* data_row = new QSurfaceDataRow(cols);
//        double x = xmin + (xmax-xmin)*i/rows;
//        for(auto j = 0; j < cols; j++){
//            double y = ymin + (ymax-ymin)*i/cols;
//            double z = exp(-(pow(x,2)+pow(y,2)));
//            (*data_row)[j].setPosition(QVector3D(x,y,z));
//        }
//        *m_data_array << data_row;
//    }
//    m_data_proxy->resetArray(m_data_array);

//    m_graph->axisX()->setLabelFormat("%.2f");
//    m_graph->axisY()->setLabelFormat("%.2f");
//    m_graph->axisZ()->setLabelFormat("%.2f");
//    m_graph->axisX()->setRange(xmin,xmax);
//    m_graph->axisY()->setRange(ymin,ymax);
//    m_graph->axisX()->setLabelAutoRotation(30);
//    m_graph->axisY()->setLabelAutoRotation(90);
//    m_graph->axisZ()->setLabelAutoRotation(30);
}

void SurfaceGraph::initTest2()
{
    const int sampleCountX = 50;
    const int sampleCountZ = 50;
    const int heightMapGridStepX = 6;
    const int heightMapGridStepZ = 6;
    const float sampleMin = -8.0f;
    const float sampleMax = 8.0f;
    float stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
    float stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);

    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(sampleCountZ);
    for (int i = 0 ; i < sampleCountZ ; i++) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleCountX);
        // Keep values within range bounds, since just adding step can cause minor drift due
        // to the rounding errors.
        float z = qMin(sampleMax, (i * stepZ + sampleMin));
        int index = 0;
        for (int j = 0; j < sampleCountX; j++) {
            float x = qMin(sampleMax, (j * stepX + sampleMin));
            float R = qSqrt(z * z + x * x) + 0.01f;
            float y = (qSin(R) / R + 0.24f) * 1.61f;
            (*newRow)[index++].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newRow;
    }

    m_data_proxy->resetArray(dataArray);
}


void SurfaceGraph::graph(const QString& fname,pw::FileSignature fsig,\
        pw::DataSignature datasig, pw::OperatorSignature opsig)
{
    qDebug() << "SurfaceGraph::graph called";
//    clearSeries();
    qDebug() << "clearSeries called";
    initTest();
//    ParamBin bin = dataaux3D::XYZToSurfaceDataArray(fname,*m_data_array,fsig);
//    m_data_proxy->resetArray(m_data_array);
//    m_graph->axisX()->setRange(bin.getDbl("min_xval"),bin.getDbl("max_xval"));
//    m_graph->axisY()->setRange(bin.getDbl("min_yval"),bin.getDbl("max_yval"));
//    m_graph->axisZ()->setRange(bin.getDbl("min_zval"),bin.getDbl("max_zval"));
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







