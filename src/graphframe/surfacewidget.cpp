
#include "graphframe/surfacewidget.h"
#include "graphframe/graphframe.h"
#include "dropwidget.h"
#include "draglistview.h"
#include "core/dataaux3D.h"

#include <QChart>
#include <QLineSeries>
#include <QString>
#include <QStringList>
#include <QStringLiteral>
#include <QVBoxLayout>
#include <Q3DSurface>
#include <QSurfaceDataProxy>
#include <QSurface3DSeries>
#include <QValue3DAxis>
#include <QValue3DAxisFormatter>
#include <QLogValue3DAxisFormatter>
#include <Q3DTheme>
#include <Q3DCamera>
#include <QFont>
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>

#include <cmath>
#include <pwutils/pwmath.hpp>
#include <ParamBin/parambin.hpp>


SurfaceWidget::SurfaceWidget(GraphFrame* parent_frame) :
    m_parent_frame(parent_frame)
{
    QHBoxLayout* hbox = new QHBoxLayout();
    m_surface_graph = new SurfaceGraph(parent_frame);

    hbox->addWidget(m_surface_graph);

    QVBoxLayout* vbox = new QVBoxLayout();

    m_axisMinSliderX = new QSlider(Qt::Horizontal);
    m_axisMinSliderX->setMinimum(0);
    m_axisMinSliderX->setMaximum(99);
    m_axisMinSliderX->setValue(0);
    m_axisMinSliderX->setTickInterval(20);
    m_axisMinSliderX->setEnabled(true);
    m_axisMaxSliderX = new QSlider(Qt::Horizontal);
    m_axisMaxSliderX->setMinimum(1);
    m_axisMaxSliderX->setMaximum(100);
    m_axisMaxSliderX->setValue(100);
    m_axisMaxSliderX->setTickInterval(20);
    m_axisMaxSliderX->setEnabled(true);
    m_axisMinSliderY = new QSlider(Qt::Horizontal);
    m_axisMinSliderY->setMinimum(0);
    m_axisMinSliderY->setMaximum(99);
    m_axisMinSliderY->setValue(0);
    m_axisMinSliderY->setTickInterval(20);
    m_axisMinSliderY->setEnabled(true);
    m_axisMaxSliderY = new QSlider(Qt::Horizontal);
    m_axisMaxSliderY->setMinimum(1);
    m_axisMaxSliderY->setMaximum(100);
    m_axisMaxSliderY->setValue(100);
    m_axisMaxSliderY->setTickInterval(20);
    m_axisMaxSliderY->setEnabled(true);

    vbox->addWidget(new QLabel(QStringLiteral("Column Range")));
    vbox->addWidget(m_axisMinSliderY);
    vbox->addWidget(m_axisMaxSliderY);
    vbox->addWidget(new QLabel(QStringLiteral("Row Range")));
    vbox->addWidget(m_axisMinSliderX);
    vbox->addWidget(m_axisMaxSliderX);

    QCheckBox* log_check_box = new QCheckBox("Logarithmic",this);
    connect(log_check_box,&QCheckBox::stateChanged,\
            [this](int state) {m_surface_graph->setLogData(state ? true : false);});
    vbox->addWidget(log_check_box);

    vbox->setAlignment(Qt::AlignTop);

    hbox->addLayout(vbox);
    setLayout(hbox);

    connect(m_axisMinSliderX,&QSlider::valueChanged,\
            this,&SurfaceWidget::changeMinX);
    connect(m_axisMaxSliderX,&QSlider::valueChanged,\
            this,&SurfaceWidget::changeMaxX);
    connect(m_axisMinSliderY,&QSlider::valueChanged,\
            this,&SurfaceWidget::changeMinY);
    connect(m_axisMaxSliderY,&QSlider::valueChanged,\
            this,&SurfaceWidget::changeMaxY);
}


void SurfaceWidget::changeMinX(int min)
{
    int max = m_axisMaxSliderX->value();
    if(min >= max){
        max = min+1;
        m_axisMaxSliderX->setValue(max);
    }
    float minVal = m_surface_graph->minX();
    float maxVal = m_surface_graph->maxX();

    m_surface_graph->setRangeX(minVal+(min/100.0)*(maxVal-minVal),\
            maxVal-(100-max)*(maxVal-minVal)/100.0);
}

void SurfaceWidget::changeMaxX(int max)
{
    int min = m_axisMinSliderX->value();
    if(max <= min){
        min = max-1;
        m_axisMinSliderX->setValue(min);
    }
    float minVal = m_surface_graph->minX();
    float maxVal = m_surface_graph->maxX();
    m_surface_graph->setRangeX(minVal+(min/100.0)*(maxVal-minVal),\
            maxVal-(100-max)*(maxVal-minVal)/100.0);
}

void SurfaceWidget::changeMinY(int min)
{
    int max = m_axisMaxSliderY->value();
    if(min >= max){
        max = min+1;
        m_axisMaxSliderY->setValue(max);
    }
    float minVal = m_surface_graph->minY();
    float maxVal = m_surface_graph->maxY();
    m_surface_graph->setRangeY(minVal+(min/100.0)*(maxVal-minVal),\
            maxVal-(100-max)*(maxVal-minVal)/100.0);
}

void SurfaceWidget::changeMaxY(int max)
{
    int min = m_axisMinSliderY->value();
    if(max <= min){
        min = max-1;
        m_axisMinSliderY->setValue(min);
    }
    float minVal = m_surface_graph->minY();
    float maxVal = m_surface_graph->maxY();
    m_surface_graph->setRangeY(minVal+(min/100.0)*(maxVal-minVal),\
            maxVal-(100-max)*(maxVal-minVal)/100.0);
}


SurfaceWidget::~SurfaceWidget()
{ }

void SurfaceWidget::graph(const QString& fname,pw::FileSignature fsig,\
            pw::DataSignature datasig, pw::OperatorSignature opsig) {

    m_surface_graph->graph(fname,fsig,datasig,opsig);
    int min = m_axisMinSliderX->value();
    int max = m_axisMaxSliderX->value();
    float minX = m_surface_graph->minX();
    float maxX = m_surface_graph->maxX();
    m_surface_graph->setRangeX(minX+(min/100.0)*(maxX-minX),\
            maxX-(100-max)*(maxX-minX)/100.0);

    min = m_axisMinSliderY->value();
    max = m_axisMaxSliderY->value();
    float minY = m_surface_graph->minY();
    float maxY = m_surface_graph->maxY();
    m_surface_graph->setRangeY(minY+(min/100.0)*(maxY-minY),\
            maxY-(100-max)*(maxY-minY)/100.0);
}



SurfaceGraph::SurfaceGraph(GraphFrame* parent_frame,QWidget* parent_widget) : 
    QWidget(parent_widget),
    m_parent_frame(parent_frame),
    m_graph(new Q3DSurface()),
    m_maxpoint2DX(200),
    m_maxpoint2DY(200),
    m_log_data(false),
    m_log_decades(10)
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
    QSize screen_size = m_graph->screen()->size();
    graph_widget->setMinimumSize(QSize(screen_size.width()/3.0,screen_size.height()/2.4));
    graph_widget->setMaximumSize(screen_size);
    graph_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    graph_widget->setFocusPolicy(Qt::StrongFocus);
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(graph_widget);
    setLayout(vbox);
}

SurfaceGraph::~SurfaceGraph()
{ }

void SurfaceGraph::setLogData(bool val)
{
    if(val == m_log_data)
        return;

    m_log_data = val;
    clearSeries();
    if(val){
        setModelXYLOGZ();
//        m_data_proxy->resetArray(m_data_array);
        m_graph->axisY()->setFormatter(new QLogValue3DAxisFormatter);
    }
    else{
        setModelXYZ();
//        m_data_proxy->resetArray(m_data_array);
        m_graph->axisY()->setFormatter(new QValue3DAxisFormatter);
    }
    m_data_proxy->resetArray(m_data_array);
    setDefaultRangeZ();
    qDebug() << "Exit setLogData";
}

float SurfaceGraph::minX() const
{
    if(m_x.empty())
        return 0.0;
    return m_x[0];
}

float SurfaceGraph::maxX() const
{
    if(m_x.empty())
        return 0.0;
    return m_x.back();
}

float SurfaceGraph::minY() const
{
    if(m_y.empty())
        return 0.0;
    return m_y[0];
}

float SurfaceGraph::maxY() const
{
    if(m_y.empty())
        return 0.0;
    return m_y.back();
}


void SurfaceGraph::setRangeX(float minX,float maxX)
{
    if(minX >= maxX)
        return;
    if(minX < m_x.front())
        minX = m_x.front();
    if(maxX > m_x.back())
        maxX = m_x.back();
    m_graph->axisZ()->setRange(minX,maxX);
}

void SurfaceGraph::setRangeY(float minY,float maxY)
{
    if(minY >= maxY)
        return;
    if(minY < m_y.front())
        minY = m_y.front();
    if(maxY > m_y.back())
        maxY = m_y.back();
    m_graph->axisX()->setRange(minY,maxY);
}

void SurfaceGraph::setRangeZ(float minZ,float maxZ)
{
    if(minZ >= maxZ)
        return;
    float min_val = pw::min(m_z);
    if(minZ < min_val)
        minZ = min_val;
    float max_val = pw::max(m_z);
    if(maxZ > max_val)
        maxZ = max_val;
    m_graph->axisY()->setRange(minZ,maxZ);
}

void SurfaceGraph::setRangeXY(float minX,float maxX,float minY,float maxY)
{
    setRangeX(minX,maxX);
    setRangeY(minY,maxY);
}

void SurfaceGraph::setDefaultRange()
{
    setRangeXY(m_x.front(),m_x.back(),m_y.front(),m_y.back());
    setDefaultRangeZ();
}

void SurfaceGraph::setDefaultRangeZ()
{
    float max_zval = pw::max(m_z);
    if(m_log_data){
        float min_zval = pow(10,-static_cast<float>(m_log_decades))*max_zval;
        setRangeZ(min_zval,max_zval);
    } else{
        float min_zval = pw::min(m_z);
        setRangeZ(min_zval,max_zval);
    }
}

void SurfaceGraph::setLabels(const ParamBin& bin)
{    
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

void SurfaceGraph::graph(const QString& fname,pw::FileSignature fsig,\
        pw::DataSignature datasig, pw::OperatorSignature opsig)
{
    if(datasig != pw::DataSignature::XYZ)
        return;

    m_fname = fname;
    m_fsig = fsig;
    m_datasig = datasig;
    m_opsig = opsig;

    clearData();
    ParamBin bin = dataaux3D::readXYZData(fname,m_x,m_y,m_z,fsig,m_maxpoint2DX,m_maxpoint2DY);
    if(m_x.empty() || m_y.empty() || m_z.empty())
        return;

    clearSeries();
    if(m_log_data){
        setModelXYLOGZ();
        float max_zval = bin.getFloat("max_zval");
        float min_zval = pow(10,-static_cast<float>(m_log_decades))*max_zval;
        bin.set("min_zval",min_zval);
    }
    else
        setModelXYZ();
    m_data_proxy->resetArray(m_data_array);
    setDefaultRange();
    setLabels(bin);
}

void SurfaceGraph::setModelXYZ()
{
    m_data_array->reserve(m_x.size());
    for(auto i = 0; i < m_x.size(); i++){
        QList<QSurfaceDataItem>* data_row = new QList<QSurfaceDataItem>(m_y.size());
        for(auto j = 0; j < m_y.size(); j++) {
            (*data_row)[j].setPosition(QVector3D(m_y[j],m_z[i*m_y.size()+j],m_x[i]));
        }
        *m_data_array << data_row;
    }
}

void SurfaceGraph::setModelXYLOGZ()
{
    m_data_array->reserve(m_x.size());
    float max_zval = pw::max(m_z);
    float min_zval = pow(10,-static_cast<float>(m_log_decades))*max_zval;

    for(auto i = 0; i < m_x.size(); i++){
        QList<QSurfaceDataItem>* data_row = new QList<QSurfaceDataItem>(m_y.size());
        for(auto j = 0; j < m_y.size(); j++) {
            double zval = m_z[i*m_y.size()+j];
            if(zval < min_zval)
                zval = min_zval;
            (*data_row)[j].setPosition(QVector3D(m_y[j],zval,m_x[i]));
        }
        *m_data_array << data_row;
    }
}



void SurfaceGraph::clearSeries()
{
    // m_data_proxy clears (deletes) m_data_array 
    m_data_proxy->resetArray(nullptr);
    m_data_array = new QSurfaceDataArray;
}

void SurfaceGraph::clearData()
{
    m_x.clear();
    m_y.clear();
    m_z.clear();
}








