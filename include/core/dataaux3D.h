//dataux3D.h
#pragma once

#include <QString>
#include <QList>
#include <vector>
#include <QSurfaceDataItem>
#include <ParamBin/parambin.hpp>
#include <pwutils/pwdefs.h>

namespace dataaux3D{
    ParamBin XYZToSurfaceDataArray(const QString& fname,\
            QList<QList<QSurfaceDataItem>*>& data_array,\
            pw::FileSignature filesig);
}

