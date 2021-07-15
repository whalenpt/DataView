#include "dataaux.h"
#include "fileaux.h"

#include <string>
#include <QString>
#include <QLineSeries>
#include <QValueAxis>
#include <QLogValueAxis>
#include <pwutils/pwmath.hpp>
#include <ParamBin/parambin.hpp>

namespace dataaux{

void twoColFileToSeries(const std::string& fname,QLineSeries& series,ParamBin& bin)
{
    series.clear();
    std::vector<double> x;
    std::vector<double> y;
    bin = fileaux::readTwoColDoubles(fname,x,y);
    double min_xval = pw::min(x);
    double max_xval = pw::max(x);
    double min_yval = pw::min(y);
    double max_yval = pw::max(y);

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

    setSeriesName(fname,series);
    bin.set("min_xval",min_xval);
    bin.set("max_xval",max_xval);
    bin.set("min_yval",min_yval);
    bin.set("max_yval",max_yval);
}

void twoColFilesToSeries(const QStringList& fnames,\
        std::vector<QLineSeries*>& line_series_vec,ParamBin& bin)
{
    for(auto item : line_series_vec)
        delete item;
    line_series_vec.clear();

    std::vector<double> min_xvals,max_xvals,min_yvals,max_yvals;
    QString fname = fnames[0];

    for(auto& fname : fnames){
        line_series_vec.push_back(new QLineSeries);
        twoColFileToSeries(fname.toStdString(),*line_series_vec.back(),bin);
        setSeriesName(fname,*line_series_vec.back());
        min_xvals.push_back(bin.getDbl("min_xval"));
        max_xvals.push_back(bin.getDbl("max_xval"));
        min_yvals.push_back(bin.getDbl("min_yval"));
        max_yvals.push_back(bin.getDbl("max_yval"));
    }
    double min_xval = pw::min(min_xvals);
    double max_xval = pw::max(max_xvals);
    double min_yval = pw::min(min_yvals);
    double max_yval = pw::max(max_yvals);
//    qDebug() << QString("Number of series recorded %1").arg(line_series_vec.size());
//    qDebug() << "Max xval" << QString::number(max_xval);
//    qDebug() << "Max yval" << QString::number(max_yval);
//    qDebug() << "Number of points in series 0: " << QString::number(line_series_vec.back()->points().size());

    bin.set("min_xval",min_xval);
    bin.set("max_xval",max_xval);
    bin.set("min_yval",min_yval);
    bin.set("max_yval",max_yval);
}

void setSeriesName(const std::string& fname,QLineSeries& series) {
    setSeriesName(QString::fromStdString(fname),series);
}

void setSeriesName(const QString& fname,QLineSeries& series) {
    series.setName(fileaux::getLocalFileName(fname));
}


void formatAxisX(const ParamBin& bin,QValueAxis& axis)
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

void formatAxisY(const ParamBin& bin,QValueAxis& axis)
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

void formatAxisLogX(const ParamBin& bin,QLogValueAxis& axis)
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

void formatAxisLogY(const ParamBin& bin,QLogValueAxis& axis)
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
