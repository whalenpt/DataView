
#include "graphgui/dropchartview.h"
#include "listgui/draglistview.h"
#include <QChart>
#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QByteArray>
#include <QDataStream>
#include <QString>


//#include <QDebug>

DropChartView::DropChartView(QChart* chart,QWidget* parent)
    : QChartView(chart,parent)
{
    setAcceptDrops(true);
}

void DropChartView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText() || \
            event->mimeData()->hasFormat(DragListView::StringListMime)){
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
//    qDebug() << "DropChartView::dropEvent";
    if(event->mimeData()->hasText())
        emit fileDrop(event->mimeData()->text());
    else if(event->mimeData()->hasFormat(DragListView::StringListMime)){
        QByteArray file_data = event->mimeData()->data(DragListView::StringListMime);
        QDataStream data_stream(&file_data,QIODevice::ReadOnly);
        QStringList filenames;
        data_stream >> filenames;
        emit fileListDrop(filenames);
    }
}

