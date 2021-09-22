
#include "core/dataaux3D.h"
#include <pwutils/read/dat.hpp>
#include <pwutils/read/json.hpp>
#include <pwutils/pwmath.hpp>
#include <cmath>
#include <iostream>
#include <ParamBin/parambin.hpp>
#include <QSurfaceDataItem>
#include <QDebug>

namespace dataaux3D{
//ParamBin XYZToSurfaceDataArray(const QString& fname,\
//        QList<QList<QSurfaceDataItem>*>& data_array,\
//        pw::FileSignature filesig,unsigned int maxpointX,unsigned int maxpointY)

ParamBin readXYZData(const QString& fname,\
        std::vector<float>& x,std::vector<float>& y,std::vector<float>& z,\
        pw::FileSignature filesig,unsigned int maxpointX,unsigned int maxpointY)
{
    x.clear();
    y.clear();
    z.clear();
    ParamBin bin;
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

    // mirror R coordinate if found
    if((bin.inBin("xcordID") && bin.getStrU("xcordID")=="R") || \
           (bin.inBin("xcordID") && bin.getStrU("xcordID")=="SR") ){
        auto nr = x.size();
        std::vector<float> xcopy(x.size());
        std::vector<float> zcopy(z.size());
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
    return bin;
}

//void logData(std::vector<float>& z,unsigned int decades,ParamBin& bin){
//    std::cout << decades << std::endl;
//    float max_zval = pw::max(z);
//    float min_zval = std::max(1e-7,pow(10,-static_cast<float>(decades))*max_zval);
//    std::cout << min_zval << std::endl;
//    std::cout << max_zval << std::endl;
//    for(auto i = 0; i < z.size(); i++){
//        if(z[i] > min_zval)
//            z[i] = log10(z[i]);
//        else
//            z[i] = min_zval;
//    }
//    bin.set("min_zval",min_zval);
//    bin.set("max_zval",max_zval);
//}
//
//    data_array.reserve(x.size());
//    for(auto i = 0; i < x.size(); i++){
//        QList<QSurfaceDataItem>* data_row = new QList<QSurfaceDataItem>(y.size());
//        for(auto j = 0; j < y.size(); j++) {
//            (*data_row)[j].setPosition(QVector3D(y[j],z[i*y.size()+j],x[i]));
//        }
//        data_array << data_row;
//    }



//    if(bin.inBin("xscale") && bin.getFloat("xscale")>0){
//        float scale = bin.getFloat("xscale");
//        for(unsigned int i = 0; i < x.size(); i++)
//            x[i] /= scale;
//        bin.set("min_xval",min_xval/scale);
//        bin.set("max_xval",max_xval/scale);
//        if(bin.inBin("xscale_str") && bin.inBin("xlabel"))
//            bin.set("xlabel",bin.getStr("xlabel") + " ["\
//                    + bin.getStr("xscale_str") + "]"); 
//    } else{ //normalize
//        float scale = max_xval-min_xval;
//        for(unsigned int i = 0; i < x.size(); i++)
//            x[i] /= scale;
//        bin.set("min_xval",min_xval/scale);
//        bin.set("max_xval",max_xval/scale);
//        if(bin.inBin("xlabel"))
//            bin.set("xlabel",bin.getStr("xlabel") + " [Arb.]");
//    }
//
//    if(bin.inBin("yscale") && bin.getFloat("yscale") > 0){
//        float scale = bin.getFloat("yscale");
//        for(unsigned int i = 0; i < y.size(); i++)
//            y[i] /= scale;
//        bin.set("min_yval",min_yval/scale);
//        bin.set("max_yval",max_yval/scale);
//        if(bin.inBin("yscale_str") && bin.inBin("ylabel"))
//            bin.set("ylabel",bin.getStr("ylabel") + " ["\
//                    + bin.getStr("yscale_str") + "]"); 
//    } else{ //normalize
//        float scale = max_yval-min_yval;
//        for(unsigned int i = 0; i < y.size(); i++)
//            y[i] /= scale;
//        bin.set("min_yval",min_yval/scale);
//        bin.set("max_yval",max_yval/scale);
//        if(bin.inBin("ylabel"))
//            bin.set("ylabel",bin.getStr("ylabel") + " [Arb.]");
//    }
//    if(bin.inBin("zscale") && bin.getFloat("zscale")>0){
//        float scale = bin.getFloat("zscale");
//        for(unsigned int i = 0; i < z.size(); i++)
//            z[i] /= scale;
//        bin.set("min_zval",min_zval/scale);
//        bin.set("max_zval",max_zval/scale);
//        if(bin.inBin("zscale_str") && bin.inBin("zlabel"))
//            bin.set("zlabel",bin.getStr("zlabel") + " ["\
//                    + bin.getStr("zscale_str") + "]"); 
//    } else{ //normalize
//        float scale = max_zval-min_zval;
//        for(unsigned int i = 0; i < z.size(); i++)
//            y[i] /= scale;
//        bin.set("min_zval",min_zval/scale);
//        bin.set("max_zval",max_zval/scale);
//        if(bin.inBin("zlabel"))
//            bin.set("zlabel",bin.getStr("zlabel") + " [Arb.]");
//    }



}



