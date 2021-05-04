#ifndef DRAGLISTVIEW_H
#define DRAGLISTVIEW_H

#include <QListView>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QStringList>
#include <filesystem>

class DragListView : public QListView
{
public:
    DragListView(const std::filesystem::path& dirpath,QWidget* parent = 0);
    std::filesystem::path getTargetDirPath() {return m_target_dirpath;}
    void setTargetDirPath(const std::filesystem::path& dirpath);
    void getFileNames(QStringList& slist);
    static QString fileMimeType() {return QStringLiteral("filename");}
    bool isDir(QModelIndex& index);

protected:
    void startDrag(Qt::DropActions supportedActions) Q_DECL_OVERRIDE;

private slots:
    void openDataFile();

private:
    std::filesystem::path m_target_dirpath;
    QFileSystemModel* m_file_model;
};


#endif // DRAGLISTVIEW_H
