#include "dataaux.h"
#include "fileaux.h"

#include <string>
#include <QString>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <pwutils/pwmath.h>
#include <parambin.hpp>

namespace dataaux{

void twoColFileToSeries(const std::string& fname,QtCharts::QLineSeries& series,ParamBin& bin)
{
    series.clear();
    std::vector<double> x;
    std::vector<double> y;
    bin = fileaux::readTwoColDoubles(fname,x,y);
    double min_xval = pw::getMin(x);
    double max_xval = pw::getMax(x);
    double min_yval = pw::getMin(y);
    double max_yval = pw::getMax(y);

    // Workaround QValueAxis setRange issue handling small numbers
    if(fabs(max_xval - min_xval) < 1.0e-12){
       double mult_fact = 1.0e15;
       min_xval *= mult_fact;
       max_xval *= mult_fact;
       for(unsigned int i = 0; i < x.size(); i++)
           series.append(mult_fact*x[i],y[i]);
    } else{
        for(unsigned int i = 0; i < x.size(); i++)
           series.append(x[i],y[i]);
    }

    bin.set("min_xval",min_xval);
    bin.set("max_xval",max_xval);
    bin.set("min_yval",min_yval);
    bin.set("max_yval",max_yval);
}

void setSeriesName(const std::string& fname,QtCharts::QLineSeries& series)
{
    QString full_filename = QString::fromStdString(fname);
    series.setName(fileaux::getLocalFileName(full_filename));
}

void formatAxisX(const ParamBin& bin,QtCharts::QValueAxis& axis)
{
    axis.setRange(bin.getDbl("min_xval"),bin.getDbl("max_xval"));
    if(bin.inBin("xlabel")){
        std::string xlabel = bin.getStr("xlabel");
        if(bin.inBin("xunit_str")){
            xlabel = xlabel + " (" + bin.getStr("xunit_str") + ")";
        }
        axis.setTitleText(QString::fromStdString(xlabel));
    }
}

void formatAxisY(const ParamBin& bin,QtCharts::QValueAxis& axis)
{
    axis.setRange(bin.getDbl("min_yval"),bin.getDbl("max_yval"));
    if(bin.inBin("ylabel")){
        std::string ylabel = bin.getStr("ylabel");
        if(bin.inBin("yunit_str")){
            ylabel = ylabel + " (" + bin.getStr("yunit_str") + ")";
        }
        axis.setTitleText(QString::fromStdString(ylabel));
    }
}

void formatAxisLogY(const ParamBin& bin,QtCharts::QLogValueAxis& axis)
{
    axis.setRange(bin.getDbl("min_yval"),bin.getDbl("max_yval"));
    if(bin.inBin("ylabel")){
        std::string ylabel = bin.getStr("ylabel");
        if(bin.inBin("yunit_str")){
            ylabel = ylabel + " (" + bin.getStr("yunit_str") + ")";
        }
        axis.setTitleText(QString::fromStdString(ylabel));
    }
}

}
