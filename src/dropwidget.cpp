
#include "dropwidget.h"
#include "draglistview.h"
#include <QChartView>
#include <QChart>
#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QByteArray>
#include <QDataStream>
#include <QString>

DropChartView::DropChartView(QChart* chart,QWidget* parent_widget) :
    QChartView(chart,parent_widget)
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
    QStringList filenames;
    if(event->mimeData()->hasText())
        filenames.append(event->mimeData()->text());
    else if(event->mimeData()->hasFormat(DragListView::StringListMime)){
        QByteArray file_data = event->mimeData()->data(DragListView::StringListMime);
        QDataStream data_stream(&file_data,QIODevice::ReadOnly);
        data_stream >> filenames;
    } else
        return;
    emit fileDrop(filenames);
}



DropWidget::DropWidget(QWidget* parent_widget) :
    QWidget(parent_widget)
{
    setAcceptDrops(true);
}

void DropWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText() || \
            event->mimeData()->hasFormat(DragListView::StringListMime)){
        event->accept();
    }
    else
        event->ignore();
}

void DropWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropWidget::dropEvent(QDropEvent* event)
{
    QStringList filenames;
    if(event->mimeData()->hasText())
        filenames.append(event->mimeData()->text());
    else if(event->mimeData()->hasFormat(DragListView::StringListMime)){
        QByteArray file_data = event->mimeData()->data(DragListView::StringListMime);
        QDataStream data_stream(&file_data,QIODevice::ReadOnly);
        data_stream >> filenames;
    } else
        return;
    emit fileDrop(filenames);
}

DropFrame::DropFrame(QWidget* parent) :
    QFrame(parent)
{
    setAcceptDrops(true);
}

void DropFrame::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText() || \
            event->mimeData()->hasFormat(DragListView::StringListMime)){
        event->accept();
    }
    else
        event->ignore();
}

void DropFrame::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropFrame::dropEvent(QDropEvent* event)
{
    QStringList filenames;
    if(event->mimeData()->hasText())
        filenames.append(event->mimeData()->text());
    else if(event->mimeData()->hasFormat(DragListView::StringListMime)){
        QByteArray file_data = event->mimeData()->data(DragListView::StringListMime);
        QDataStream data_stream(&file_data,QIODevice::ReadOnly);
        data_stream >> filenames;
    } else
        return;
    emit fileDrop(filenames);
}




