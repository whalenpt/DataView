#ifndef DATAVIEW_H
#define DATAVIEW_H

#include <QDialog>
#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QModelIndex>
#include <QCloseEvent>
#include "dataview_global.h"

class GraphFrame;
class DragListView;

class DATAVIEWSHARED_EXPORT DataView : public QDialog
{
    Q_OBJECT

public:
    explicit DataView(QWidget *parent = 0);
    void setTargetPath(const QString& c_target_path);
    ~DataView();

private slots:
    void graphIfFile(QModelIndex index);
    void dir_pushButton_clicked();

private:

    QLabel* dir_name;
    QPushButton* dir_pushButton;
    DragListView* list_view;
    GraphFrame* graph_frame;
    QString target_path;

    void init();
//    void writeSettings();
//    void readSettings();
};

#endif // DATAVIEW_H
