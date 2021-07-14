
#ifndef AXESAUX_H
#define AXESAUX_H

enum class AxesType {Standard,Semilogy,Semilogx,Loglog};
class QChart;
class QAbstractSeries; 
class QValueAxis;
class QLogValueAxis;

namespace axesaux{
    void setAxes(AxesType axes_type,QChart* chart,QAbstractSeries* series,\
            QList<QValueAxis*> axes,QList<QLogValueAxis*> logaxes)
}

