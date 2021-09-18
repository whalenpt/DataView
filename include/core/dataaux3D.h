//dataux3D.h
#pragma once

#include <QString>
#include <QList>
#include <vector>
#include <QSurfaceDataItem>
#include <ParamBin/parambin.hpp>
#include <pwutils/pwdefs.h>

namespace dataaux3D{
//    ParamBin XYZToSurfaceDataArray(const QString& fname,\
//            QList<QList<QSurfaceDataItem>*>& data_array,\
//            pw::FileSignature filesig,unsigned int maxpointX,unsigned int maxpointY);
    ParamBin readXYZData(const QString& fname,\
        std::vector<float>& x,std::vector<float>& y,std::vector<float>& z,\
            pw::FileSignature filesig,unsigned int maxpointX,unsigned int maxpointY);
    void fillSurfaceDataItems(QList<QList<QSurfaceDataItem>*>& data_array,\
        const std::vector<float>& x,const std::vector<float>& y,const std::vector<float>& z);
}

