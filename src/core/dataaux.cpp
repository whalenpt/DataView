#include "core/dataaux.h"
#include <string>
#include <QString>
#include <QLineSeries>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QFileInfo>
#include <pwutils/pwmath.hpp>
#include <pwutils/read/readdat.h>
#include <pwutils/read/readjson.h>
#include <ParamBin/parambin.hpp>

namespace dataaux{

ParamBin XYToSeries(const QString& fname,QLineSeries& series,pw::FileSignature filesig)
{
    series.clear();
    series.setUseOpenGL(true);
    std::vector<double> x;
    std::vector<double> y;
    ParamBin bin;
    if(filesig == pw::FileSignature::DAT)
        bin = ParamBin({dat::readXY(fname.toStdString(),x,y)});
    else if(filesig == pw::FileSignature::JSON)
        bin = ParamBin({json::readXY(fname.toStdString(),x,y)});
    else
        return ParamBin();

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
    return bin;
}

ParamBin multiXYToSeries(const QStringList& fnames,\
        std::vector<QLineSeries*>& line_series_vec,pw::FileSignature filesig)
{
    for(auto item : line_series_vec)
        delete item;
    line_series_vec.clear();

    std::vector<double> min_xvals,max_xvals,min_yvals,max_yvals;
    ParamBin bin;
    for(auto& fname : fnames){
        line_series_vec.push_back(new QLineSeries);
        bin = XYToSeries(fname,*line_series_vec.back(),filesig);
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

    bin.set("min_xval",min_xval);
    bin.set("max_xval",max_xval);
    bin.set("min_yval",min_yval);
    bin.set("max_yval",max_yval);
    return bin;
}

ParamBin XY_CToSeries(const QString& fname,QLineSeries& series1,QLineSeries& series2,pw::FileSignature filesig)
{
    series1.clear();
    series1.setUseOpenGL(true);
    series2.clear();
    series2.setUseOpenGL(true);

    std::vector<double> x;
    std::vector<double> y1;
    std::vector<double> y2;
    ParamBin bin;
    if(filesig == pw::FileSignature::DAT)
        bin = ParamBin({dat::readXCVY(fname.toStdString(),x,y1,y2)});
    else if(filesig == pw::FileSignature::JSON)
        bin = ParamBin({json::readXCVY(fname.toStdString(),x,y1,y2)});
    else
        return ParamBin();

    double min_xval = pw::min(x);
    double max_xval = pw::max(x);
    double min_yval1 = pw::min(y1);
    double max_yval1 = pw::max(y1);
    double min_yval2 = pw::min(y2);
    double max_yval2 = pw::max(y2);

    // Workaround QValueAxis setRange issue handling small numbers
    if(fabs(max_xval - min_xval) < 1.0e-12){
       double mult_fact = 1.0e15;
       min_xval *= mult_fact;
       max_xval *= mult_fact;
       for(auto i = 0; i < x.size(); i++){
           series1.append(mult_fact*x[i],y1[i]);
           series2.append(mult_fact*x[i],y2[i]);
       }
    } else{
        for(auto i = 0; i < x.size(); i++){
           series1.append(x[i],y1[i]);
           series2.append(x[i],y2[i]);
        }
    }
    series1.setName(getLocalFileName(fname) + " - real");
    series2.setName(getLocalFileName(fname) + " - imag");
    bin.set("min_xval",min_xval);
    bin.set("max_xval",max_xval);
    bin.set("min_yval1",min_yval1);
    bin.set("max_yval1",max_yval1);
    bin.set("min_yval2",min_yval2);
    bin.set("max_yval2",max_yval2);
    return bin;
}

QString getLocalFileName(const QString& full_filename)
{
    QFileInfo file_info(full_filename);
    return file_info.fileName();
}

void setSeriesName(const QString& fname,QLineSeries& series) {
    series.setName(getLocalFileName(fname));
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
