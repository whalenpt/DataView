
#include "axesaux.h"
#include <QChart>
#include <QAbstractSeries>
#include <QLineSeries>
#include <QAbstractAxis>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QLineSeries>
#include <QList>
#include <QDebug>

namespace axesaux{

void setAxes(AxesType axes_type,QChart* chart,QLineSeries* series,\
        QList<QValueAxis*> axes,QList<QLogValueAxis*> logaxes)
{
    // axes[0] -> QValueAxis (for x-axis)
    // axes[1] -> QValueAxis (for y-axis)
    // logaxes[0] -> QLogValueAxis (for x-axis)
    // logaxes[1] -> QLogValueAxis (for y-axis)
    if(axes.size() != 2 || logaxes.size() != 2){
        qDebug() << "Expected 2 axes and 2 log axes in function.";
        return;
    }

    QList<QAbstractAxis*> x_axes = chart->axes(Qt::Horizontal,series);
    QList<QAbstractAxis*> y_axes = chart->axes(Qt::Vertical,series);
    for(auto axis : x_axes)
        chart->removeAxis(axis);
    for(auto axis : y_axes)
        chart->removeAxis(axis);

//    chart->removeAxis(chart->axes(Qt::Horizontal,series).at(0));
//    chart->removeAxis(chart->axes(Qt::Vertical,series).at(0));
    for(auto item : series->attachedAxes())
        series->detachAxis(item);

    if(axes_type == AxesType::Standard && axes.size() >= 2){
        chart->addAxis(axes[0],Qt::AlignBottom);
        chart->addAxis(axes[1],Qt::AlignLeft);
        series->attachAxis(axes[0]);
        series->attachAxis(axes[1]);
    } else if(axes_type == AxesType::Semilogy && axes.size() >= 4) {
        chart->addAxis(axes[0],Qt::AlignBottom);
        chart->addAxis(logaxes[1],Qt::AlignLeft);
        series->attachAxis(axes[0]);
        series->attachAxis(logaxes[1]);
    } else if(axes_type == AxesType::Semilogx && axes.size() >=3) {
        chart->addAxis(logaxes[0],Qt::AlignBottom);
        chart->addAxis(axes[1],Qt::AlignLeft);
        series->attachAxis(logaxes[0]);
        series->attachAxis(axes[1]);
    } else if(axes_type == AxesType::Loglog){
        chart->addAxis(logaxes[0],Qt::AlignBottom);
        chart->addAxis(logaxes[1],Qt::AlignLeft);
        series->attachAxis(logaxes[0]);
        series->attachAxis(logaxes[1]);
    }
}

void setAxes(AxesType axes_type,QChart* chart,std::vector<QLineSeries*>& series_vec,\
        QList<QValueAxis*> axes,QList<QLogValueAxis*> logaxes)
{
    // axes[0] -> QValueAxis (for x-axis)
    // axes[1] -> QValueAxis (for y-axis)
    // logaxes[0] -> QLogValueAxis (for x-axis)
    // logaxes[1] -> QLogValueAxis (for y-axis)
    
    if(axes.size() != 2 || logaxes.size() != 2){
        qDebug() << "Expected 2 axes and 2 log axes in function.";
        return;
    }

    QList<QAbstractAxis*> x_axes = chart->axes(Qt::Horizontal);
    QList<QAbstractAxis*> y_axes = chart->axes(Qt::Vertical);
    for(auto axis : x_axes)
        chart->removeAxis(axis);
    for(auto axis : y_axes)
        chart->removeAxis(axis);

//    chart->removeAxis(chart->axes(Qt::Horizontal,series).at(0));
//    chart->removeAxis(chart->axes(Qt::Vertical,series).at(0));
    for(auto series : series_vec)
        for(auto item : series->attachedAxes())
            series->detachAxis(item);

    if(axes_type == AxesType::Standard && axes.size() >= 2){
        chart->addAxis(axes[0],Qt::AlignBottom);
        chart->addAxis(axes[1],Qt::AlignLeft);
        for(auto series : series_vec){
            series->attachAxis(axes[0]);
            series->attachAxis(axes[1]);
        }
    } else if(axes_type == AxesType::Semilogy && axes.size() >= 4) {
        chart->addAxis(axes[0],Qt::AlignBottom);
        chart->addAxis(logaxes[1],Qt::AlignLeft);
        for(auto series : series_vec){
            series->attachAxis(axes[0]);
            series->attachAxis(logaxes[1]);
        }
    } else if(axes_type == AxesType::Semilogx && axes.size() >=3) {
        chart->addAxis(logaxes[0],Qt::AlignBottom);
        chart->addAxis(axes[1],Qt::AlignLeft);
        for(auto series : series_vec){
            series->attachAxis(logaxes[0]);
            series->attachAxis(axes[1]);
        }
    } else if(axes_type == AxesType::Loglog){
        chart->addAxis(logaxes[0],Qt::AlignBottom);
        chart->addAxis(logaxes[1],Qt::AlignLeft);
        for(auto series : series_vec){
            series->attachAxis(logaxes[0]);
            series->attachAxis(logaxes[1]);
        }
    }
}




}





