//dataux.h
#pragma once

#include <QString>
#include <vector>
#include <ParamBin/parambin.hpp>
#include <pwutils/pwdefs.h>
class QValueAxis;
class QLogValueAxis;
class QLineSeries;

namespace dataaux{
    ParamBin XYToSeries(const QString& fname,QLineSeries& series,pw::FileSignature filesig);
    ParamBin XY_CToSeries(const QString& fname,QLineSeries& series1,\
            QLineSeries& series2,pw::FileSignature filesig);
    ParamBin multiXYToSeries(const QStringList& fnames,\
        std::vector<QLineSeries*>& line_series_vec,pw::FileSignature filesig);

    void setSeriesName(const QString& fname,QLineSeries& series);
    QString getLocalFileName(const QString& full_filename);
    void formatAxisX(const ParamBin& bin,QValueAxis& axis);
    void formatAxisY(const ParamBin& bin,QValueAxis& axis);
    void formatAxisLogX(const ParamBin& bin,QLogValueAxis& axis);
    void formatAxisLogY(const ParamBin& bin,QLogValueAxis& axis);
}

