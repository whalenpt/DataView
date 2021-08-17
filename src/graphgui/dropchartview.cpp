
#include "graphgui/dropchartview.h"
#include "graphgui/graphframe.h"
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
#include <QPainter>


//#include <QDebug>

DropChartView::DropChartView(QChart* chart,GraphFrame* graph_frame)
    : QChartView(chart,graph_frame->parentWidget()),
    m_graph_frame(graph_frame)
{
    setAcceptDrops(true);
    setRenderHint(QPainter::Antialiasing);
    connect(this,&DropChartView::fileListDrop,
            m_graph_frame,&GraphFrame::graphFiles);
}

void DropChartView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText() || \
            event->mimeData()->hasFormat(DragListView::StringListMime)){
        qDebug() << "DropChartView::dragEnterEvent called.";
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
    QStringList filenames;
    if(event->mimeData()->hasText())
        filenames.append(event->mimeData()->text());
    else if(event->mimeData()->hasFormat(DragListView::StringListMime)){
        qDebug() << "DropChartView multi file drop event";
        QByteArray file_data = event->mimeData()->data(DragListView::StringListMime);
        QDataStream data_stream(&file_data,QIODevice::ReadOnly);
        data_stream >> filenames;
    } else
        return;
    emit fileListDrop(filenames);
}

