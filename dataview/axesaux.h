
#ifndef AXESAUX_H
#define AXESAUX_H

#include <QList>
#include <vector>
enum class AxesType {Standard,Semilogy,Semilogx,Loglog};
class QChart;
class QValueAxis;
class QLogValueAxis;
class QLineSeries;



namespace axesaux{
    void setAxes(AxesType axes_type,QChart* chart,QLineSeries* series,\
            QList<QValueAxis*> axes,QList<QLogValueAxis*> logaxes);

    void setAxes(AxesType axes_type,QChart* chart,std::vector<QLineSeries*>& series_vec,\
            QList<QValueAxis*> axes,QList<QLogValueAxis*> logaxes);
}


#endif // AXESAUX_H

