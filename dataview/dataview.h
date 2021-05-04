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
#include <filesystem>
#include "dataview_global.h"

class GraphFrame;
class DragListView;

class DATAVIEWSHARED_EXPORT DataView : public QDialog
{
    Q_OBJECT

public:
    explicit DataView(QWidget *parent = 0);
    void setTargetDirPath(const std::filesystem::path& path);
    ~DataView();

private slots:
    void graphIfFile(QModelIndex index);
    void dir_pushButton_clicked();

private:

    std::filesystem::path m_target_dirpath;
    GraphFrame* m_graph_frame;
    DragListView* m_list_view;
    QLabel* m_dir_name;
    QPushButton* m_dir_pushButton;

    void init();
};

#endif // DATAVIEW_H
