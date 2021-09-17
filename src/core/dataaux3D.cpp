
#include "core/dataaux3D.h"
#include <pwutils/read/dat.hpp>
#include <pwutils/read/json.hpp>
#include <pwutils/pwmath.hpp>
#include <iostream>
#include <ParamBin/parambin.hpp>
#include <QSurfaceDataItem>
#include <QDebug>

namespace dataaux3D{

ParamBin XYZToSurfaceDataArray(const QString& fname,\
        QList<QList<QSurfaceDataItem>*>& data_array,\
        pw::FileSignature filesig,unsigned int maxpointX,unsigned int maxpointY)
{
    ParamBin bin;
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;
    if(filesig == pw::FileSignature::DAT)
        bin = ParamBin({dat::readXYZ<float,float,float>(fname.toStdString(),x,y,z)});
    else if(filesig == pw::FileSignature::JSON)
        bin = ParamBin({json::readXYZ<float,float,float>(fname.toStdString(),x,y,z)});
    else
        return ParamBin();

    if(x.size() > maxpointX){
        auto nx = x.size();
        unsigned int strideX = static_cast<unsigned int>(x.size()/maxpointX)+1;
        unsigned int count = 0;
        for(auto i = 0; i < nx; i+=strideX){
            x[count] = x[i];
            count++;
        }
        x.resize(count);

        count = 0;
        for(auto i = 0; i < nx; i+=strideX){
            for(auto j = 0; j < y.size(); j++){
                z[count*y.size()+j] = z[i*y.size()+j];
            }
            count++;
        }
        z.resize(x.size()*y.size());
    }

    if(y.size() > maxpointY) {
        auto ny = y.size();
        unsigned int strideY = static_cast<unsigned int>(y.size()/maxpointY)+1;
        int count = 0;
        for(auto i = 0; i < ny; i+=strideY){
            y[count] = y[i];
            count++;
        }
        y.resize(count);
        for(auto i = 0; i < x.size(); i++){
            count = 0;
            for(auto j = 0; j < ny; j+=strideY){
                z[i*y.size()+count] = z[i*ny+j];
                count++;
            }
        }
        z.resize(x.size()*y.size());
    }
    std::cout << bin;

    // mirror R coordinate if found
    if((bin.inBin("xcordID") && bin.getStrU("xcordID")=="R") || \
           (bin.inBin("xcordID") && bin.getStrU("xcordID")=="SR") ){
        auto nr = x.size();
        std::vector<double> xcopy(x.size());
        std::vector<double> zcopy(z.size());
        std::copy(x.cbegin(),x.cend(),xcopy.begin());
        std::copy(z.cbegin(),z.cend(),zcopy.begin());
        x.resize(2*x.size());
        z.resize(2*z.size());
        for(auto i = 0; i < nr; i++)
            x[i] = -xcopy[nr-i-1];
        for(auto i = 0; i < nr; i++)
            x[nr+i] = xcopy[i];
        for(auto i = 0; i < nr; i++)
            for(auto j = 0; j < y.size(); j++)
                z[i*y.size()+j] = zcopy[(nr-i-1)*y.size()+j];
         for(auto i = 0; i < nr; i++)
            for(auto j = 0; j < y.size(); j++)
                z[(nr+i)*y.size()+j] = zcopy[i*y.size()+j];
    }
           
    float min_xval = pw::min(x);
    float max_xval = pw::max(x);
    float min_yval = pw::min(y);
    float max_yval = pw::max(y);
    float min_zval = pw::min(z);
    float max_zval = pw::max(z);

    bin.set("min_xval",min_xval);
    bin.set("max_xval",max_xval);
    bin.set("min_yval",min_yval);
    bin.set("max_yval",max_yval);
    bin.set("min_zval",min_zval);
    bin.set("max_zval",max_zval);

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
    if(bin.inBin("zscale") && bin.getFloat("zscale")>0){
        float scale = bin.getFloat("zscale");
        for(unsigned int i = 0; i < z.size(); i++)
            z[i] /= scale;
        bin.set("min_zval",min_zval/scale);
        bin.set("max_zval",max_zval/scale);
        if(bin.inBin("zscale_str") && bin.inBin("zlabel"))
            bin.set("zlabel",bin.getStr("zlabel") + " ["\
                    + bin.getStr("zscale_str") + "]"); 
    } else{ //normalize
        float scale = max_zval-min_zval;
        for(unsigned int i = 0; i < z.size(); i++)
            y[i] /= scale;
        bin.set("min_zval",min_zval/scale);
        bin.set("max_zval",max_zval/scale);
        if(bin.inBin("zlabel"))
            bin.set("zlabel",bin.getStr("zlabel") + " [Arb.]");
    }

    data_array.reserve(x.size());
    for(auto i = 0; i < x.size(); i++){
        QList<QSurfaceDataItem>* data_row = new QList<QSurfaceDataItem>(y.size());
        for(auto j = 0; j < y.size(); j++) {
            (*data_row)[j].setPosition(QVector3D(y[j],z[i*y.size()+j],x[i]));
        }
        data_array << data_row;
    }
    return bin;
}

}


/*
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
        bin = ParamBin({dat::readXY_C(fname.toStdString(),x,y1,y2)});
    else if(filesig == pw::FileSignature::JSON)
        bin = ParamBin({json::readXY_C(fname.toStdString(),x,y1,y2)});
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
*/

