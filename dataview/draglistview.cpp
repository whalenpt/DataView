#include "draglistview.h"

#include <QDrag>
//#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QModelIndex>
#include <QDebug>
#include <QAction>

DragListView::DragListView(const QString& c_target_path,QWidget* parent) :
    QListView(parent),
    target_path(c_target_path)
{
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    file_model = new QFileSystemModel(this);
    file_model->setRootPath(target_path);
    file_model->setFilter(QDir::NoDotAndDotDot|QDir::Files);

    setModel(file_model);
    setRootIndex(file_model->index(target_path));

    QAction* open_data_action = new QAction("Open data file");
    addAction(open_data_action);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(open_data_action,&QAction::triggered,this,&DragListView::openDataFile);
}

void DragListView::openDataFile()
{
    qDebug() << "Open data file action";
}

//void DragListView::mousePressEvent(QMouseEvent* event)
//{
//    if(event->button() == Qt::RightButton)
//        emit rightMouseClick();
//    else
//        QListView::mousePressEvent(event);
//}

void DragListView::startDrag(Qt::DropActions /*supportedActions*/)
{
    QByteArray item_data;
    QDataStream data_stream(&item_data,QIODevice::WriteOnly);

    QStringList slist;
    getFileNames(slist);
    data_stream << slist;

    QMimeData* mime_data = new QMimeData();
    mime_data->setData(DragListView::fileMimeType(),item_data);

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mime_data);
    drag->exec(Qt::CopyAction);
}

void DragListView::getFileNames(QStringList& slist){
    QModelIndexList ilist = selectionModel()->selectedIndexes();
    foreach(const QModelIndex& index,ilist){
        QString local_file_name = index.data(Qt::DisplayRole).toString();
        QString file_name = target_path + local_file_name;
        slist.append(file_name);
    }
}

void DragListView::setTargetPath(const QString& c_target_path) {
    target_path = c_target_path;
    setRootIndex(file_model->index(target_path));
}

bool DragListView::isDir(QModelIndex& index)
{
    if(file_model->isDir(index))
        return true;
    return false;
}
