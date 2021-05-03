#ifndef DATAAUX_H
#define DATAAUX_H

#include <string>
#include <parambin.hpp>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>

namespace dataaux{
    void twoColFileToSeries(const std::string& fname,QtCharts::QLineSeries& series,ParamBin& bin);
    void setSeriesName(const std::string& fname,QtCharts::QLineSeries& series);
    void formatAxisX(const ParamBin& bin,QtCharts::QValueAxis& axis);
    void formatAxisY(const ParamBin& bin,QtCharts::QValueAxis& axis);
    void formatAxisLogY(const ParamBin& bin,QtCharts::QLogValueAxis& axis);
}

#endif // DATAAUX_H
