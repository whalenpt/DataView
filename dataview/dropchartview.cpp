#include "dropchartview.h"
#include "draglistview.h"
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <QMimeData>
#include <QDebug>

DropChartView::DropChartView(QtCharts::QChart* chart,QWidget* parent)
    : QtCharts::QChartView(chart,parent)
{
    setAcceptDrops(true);
}

void DropChartView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "DropChartView::dragEnterEvent";
    if(event->mimeData()->hasFormat(DragListView::fileMimeType())){
        event->accept();
    }
    else
        event->ignore();
}

void DropChartView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropChartView::dropEvent(QDropEvent* event)
{
    qDebug() << "DropChartView::dropEvent";
    if(event->mimeData()->hasFormat(DragListView::fileMimeType())){
        QByteArray file_data = event->mimeData()->data(DragListView::fileMimeType());
        QDataStream data_stream(&file_data,QIODevice::ReadOnly);
        QStringList filenames;
        data_stream >> filenames;
        emit fileDrop(filenames);
    }
}

