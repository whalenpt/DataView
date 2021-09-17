#include "core/dataaux.h"
#include <string>
#include <QString>
#include <QLineSeries>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QFileInfo>
#include <pwutils/pwmath.hpp>
#include <pwutils/read/dat.hpp>
#include <pwutils/read/json.hpp>
#include <ParamBin/parambin.hpp>

namespace dataaux{

ParamBin XYToSeries(const QString& fname,QLineSeries& series,pw::FileSignature filesig)
{
    series.clear();
    series.setUseOpenGL(true);
    std::vector<float> x;
    std::vector<float> y;
    ParamBin bin;
    if(filesig == pw::FileSignature::DAT)
        bin = ParamBin({dat::readXY<float,float>(fname.toStdString(),x,y)});
    else if(filesig == pw::FileSignature::JSON)
        bin = ParamBin({json::readXY<float,float>(fname.toStdString(),x,y)});
    else
        return ParamBin();

    float min_xval = pw::min(x);
    float max_xval = pw::max(x);
    float min_yval = pw::min(y);
    float max_yval = pw::max(y);
    bin.set("min_xval",min_xval);
    bin.set("max_xval",max_xval);
    bin.set("min_yval",min_yval);
    bin.set("max_yval",max_yval);

    // Workaround QValueAxis setRange issue handling small numbers
    if(bin.inBin("xscale") && bin.getFloat("xscale")>0){
        float scale = bin.getFloat("xscale");
        for(unsigned int i = 0; i < x.size(); i++)
            x[i] /= scale;
        bin.set("min_xval",min_xval/scale);
        bin.set("max_xval",max_xval/scale);
        if(bin.inBin("xscale_str") && bin.inBin("xlabel"))
            bin.set("xlabel",bin.getStr("xlabel") + " ["\
                    + bin.getStr("xscale_str") + "]"); 
    } else{ //normalize
        float scale = max_xval-min_xval;
        for(unsigned int i = 0; i < x.size(); i++)
            x[i] /= scale;
        bin.set("min_xval",min_xval/scale);
        bin.set("max_xval",max_xval/scale);
        if(bin.inBin("xlabel"))
            bin.set("xlabel",bin.getStr("xlabel") + " [Arb.]");
    }

    if(bin.inBin("yscale") && bin.getFloat("yscale") > 0){
        float scale = bin.getFloat("yscale");
        for(unsigned int i = 0; i < y.size(); i++)
            y[i] /= scale;
        bin.set("min_yval",min_yval/scale);
        bin.set("max_yval",max_yval/scale);
        if(bin.inBin("yscale_str") && bin.inBin("ylabel"))
            bin.set("ylabel",bin.getStr("ylabel") + " ["\
                    + bin.getStr("yscale_str") + "]"); 
    } else{ //normalize
        float scale = max_yval-min_yval;
        for(unsigned int i = 0; i < y.size(); i++)
            y[i] /= scale;
        bin.set("min_yval",min_yval/scale);
        bin.set("max_yval",max_yval/scale);
        if(bin.inBin("ylabel"))
            bin.set("ylabel",bin.getStr("ylabel") + " [Arb.]");
    }

    for(unsigned int i = 0; i < x.size(); i++)
        series.append(x[i],y[i]);

    setSeriesName(fname,series);
    return bin;
}

ParamBin multiXYToSeries(const QStringList& fnames,\
        std::vector<QLineSeries*>& line_series_vec,pw::FileSignature filesig)
{
    for(auto item : line_series_vec)
        delete item;
    line_series_vec.clear();

    std::vector<float> min_xvals,max_xvals,min_yvals,max_yvals;
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
    float min_xval = pw::min(min_xvals);
    float max_xval = pw::max(max_xvals);
    float min_yval = pw::min(min_yvals);
    float max_yval = pw::max(max_yvals);

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

    std::vector<float> x;
    std::vector<std::complex<float>> y;
    ParamBin bin;
    if(filesig == pw::FileSignature::DAT)
        bin = ParamBin({dat::readXCVY<float,float>(fname.toStdString(),x,y)});
    else if(filesig == pw::FileSignature::JSON)
        bin = ParamBin({json::readXCVY<float,float>(fname.toStdString(),x,y)});
    else
        return ParamBin();

    float min_xval = pw::min(x);
    float max_xval = pw::max(x);
    float min_yval = abs(pw::min(y));
    float max_yval = abs(pw::max(y));
    bin.set("min_xval",min_xval);
    bin.set("max_xval",max_xval);
    bin.set("min_yval",min_yval);
    bin.set("max_yval",max_yval);

    // Workaround QValueAxis setRange issue handling small numbers
    if(bin.inBin("xscale") && bin.getFloat("xscale")>0){
        float scale = bin.getFloat("xscale");
        for(unsigned int i = 0; i < x.size(); i++)
            x[i] /= scale;
        bin.set("min_xval",min_xval/scale);
        bin.set("max_xval",max_xval/scale);
        if(bin.inBin("xscale_str") && bin.inBin("xlabel"))
            bin.set("xlabel",bin.getStr("xlabel") + " ["\
                    + bin.getStr("xscale_str") + "]"); 
    } else{ //normalize
        float scale = max_xval-min_xval;
        for(unsigned int i = 0; i < x.size(); i++)
            x[i] /= scale;
        bin.set("min_xval",min_xval/scale);
        bin.set("max_xval",max_xval/scale);
        if(bin.inBin("xlabel"))
            bin.set("xlabel",bin.getStr("xlabel") + " [Arb.]");
    }

    if(bin.inBin("yscale") && bin.getFloat("yscale") > 0){
        float scale = bin.getFloat("yscale");
        for(unsigned int i = 0; i < y.size(); i++)
            y[i] /= scale;
        bin.set("min_yval",min_yval/scale);
        bin.set("max_yval",max_yval/scale);
        if(bin.inBin("yscale_str") && bin.inBin("ylabel"))
            bin.set("ylabel",bin.getStr("ylabel") + " ["\
                    + bin.getStr("yscale_str") + "]"); 
    } else{ //normalize
        float scale = max_yval-min_yval;
        for(unsigned int i = 0; i < y.size(); i++)
            y[i] /= scale;
        bin.set("min_yval",min_yval/scale);
        bin.set("max_yval",max_yval/scale);
        if(bin.inBin("ylabel"))
            bin.set("ylabel",bin.getStr("ylabel") + " [Arb.]");
    }

    for(auto i = 0; i < x.size(); i++){
       series1.append(x[i],y[i].real());
       series2.append(x[i],y[i].imag());
    }

    series1.setName(getLocalFileName(fname) + " - real");
    series2.setName(getLocalFileName(fname) + " - imag");
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
