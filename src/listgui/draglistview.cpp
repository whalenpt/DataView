#include "listgui/draglistview.h"
#include <QDrag>
#include <QMimeData>
#include <QByteArray>
#include <QDropEvent>
#include <QModelIndex>
#include <QDebug>
#include <QAction>
#include <QDesktopServices>
#include <QMessageBox>
#include <filesystem>

const QString DragListView::StringListMime = QStringLiteral("stringlist");

DragListView::DragListView(const std::filesystem::path& dirpath,QWidget* parent) :
    QListView(parent)
{
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_file_model = new QFileSystemModel(this);
    m_file_model->setFilter(QDir::NoDotAndDotDot|QDir::Files);
    setModel(m_file_model);
    setTargetDirPath(dirpath);

    QAction* open_data_action = new QAction("Open data file");
    addAction(open_data_action);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(open_data_action,&QAction::triggered,this,&DragListView::openDataFile);
}

void DragListView::openDataFile()
{
    QStringList list;
    getFileNames(list);
    if(list.size() > 10){
        QMessageBox lottadata_msgbox;
        lottadata_msgbox.setText(QString("Are you sure you want to open %1 data files at once?")\
                .arg(QString::number(list.size())));
        lottadata_msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        lottadata_msgbox.setDefaultButton(QMessageBox::No);
        lottadata_msgbox.setIcon(QMessageBox::Warning);
        int ret = lottadata_msgbox.exec();
        if(ret == QMessageBox::No)
            return;
    } 
    int count = 0;
    for(auto& item : list){
        qDebug() << item;
        QUrl file = QUrl::fromLocalFile(item);
        if(!QDesktopServices::openUrl(file)){
            QMessageBox::information(0,QString("File open failure"),
                    QString("Failed to open the file %1").arg(item));
            count++;
        }
        if(count > 3){
            QMessageBox::critical(0,QString("Multiple file open failures"),
                    QString("Failed to open several specified files. Aborting!");
            return;
        }
    }
}

void DragListView::startDrag(Qt::DropActions /*supportedActions*/)
{
    QStringList filelist;
    getFileNames(filelist);
    if(filelist.empty())
        return;

    QMimeData* mime_data = new QMimeData();
    if(filelist.size() == 1)
        mime_data->setText(filelist[0]);
    else {
        QByteArray item_data;
        QDataStream data_stream(&item_data,QIODevice::WriteOnly);
        data_stream << filelist;
        mime_data->setData(DragListView::StringListMime,item_data);
    }
    QDrag* drag = new QDrag(this);
    drag->setMimeData(mime_data);
    drag->exec(Qt::CopyAction);
}

void DragListView::getFileNames(QStringList& filelist)
{
    QModelIndexList ilist = selectionModel()->selectedIndexes();
    for(const auto& index : ilist){
        QString local_file_name = index.data(Qt::DisplayRole).toString();
        std::filesystem::path full_path = m_target_dirpath / std::filesystem::path(local_file_name.toStdString());
        filelist.append(QString::fromStdString(full_path.string()));
    }
}

void DragListView::setTargetDirPath(const std::filesystem::path& dirpath) {
    m_target_dirpath = dirpath;
    m_file_model->setRootPath(QString::fromStdString(m_target_dirpath.string()));
    setRootIndex(m_file_model->index(QString::fromStdString(m_target_dirpath.string())));
}

bool DragListView::isDir(QModelIndex& index)
{
    if(m_file_model->isDir(index))
        return true;
    return false;
}




