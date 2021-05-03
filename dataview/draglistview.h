#ifndef DRAGLISTVIEW_H
#define DRAGLISTVIEW_H

#include <QListView>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QStringList>

class DragListView : public QListView
{
public:
    DragListView(const QString& c_target_path,QWidget* parent = 0);
    QString getTargetPath() {return target_path;}
    void setTargetPath(const QString& c_target_path);
    void getFileNames(QStringList& slist);
    static QString fileMimeType() {return QStringLiteral("filename");}
    bool isDir(QModelIndex& index);

protected:
    void startDrag(Qt::DropActions supportedActions) Q_DECL_OVERRIDE;

private slots:
    void openDataFile();

//private:
//    void mousePressEvent(QMouseEvent* event) override;

private:
    QString target_path;
    QFileSystemModel* file_model;
};


#endif // DRAGLISTVIEW_H
