#ifndef DATAAUX_H
#define DATAAUX_H

#include <string>
#include <ParamBin/parambin.hpp>
#include <QLineSeries>
#include <QValueAxis>
#include <QLogValueAxis>

namespace dataaux{
    void twoColFileToSeries(const std::string& fname,QLineSeries& series,ParamBin& bin);
    void setSeriesName(const std::string& fname,QLineSeries& series);
    void formatAxisX(const ParamBin& bin,QValueAxis& axis);
    void formatAxisY(const ParamBin& bin,QValueAxis& axis);
    void formatAxisLogX(const ParamBin& bin,QLogValueAxis& axis);
    void formatAxisLogY(const ParamBin& bin,QLogValueAxis& axis);
}

#endif // DATAAUX_H
