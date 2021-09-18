// dataview.h
#pragma once

#include <QDialog>
#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QModelIndex>
#include <QCloseEvent>
#include <QChart>
#include <filesystem>
#include "dataview_global.h"
#include "graphframe/graphframe.h"
#include "draglistview.h"

class DATAVIEWSHARED_EXPORT DataView : public QDialog
{
    Q_OBJECT

public:
    explicit DataView(QWidget *parent = 0);
    void setTargetDirPath(const std::filesystem::path& path);
    void setMaxPoint1D(unsigned int points) {m_graph_frame->setMaxPoint1D(points);}
    void setMaxPoint2DX(unsigned int points) {m_graph_frame->setMaxPoint2DX(points);}
    void setMaxPoint2DY(unsigned int points) {m_graph_frame->setMaxPoint2DY(points);}
    ~DataView();

private slots:
    void doubleClickedResponse(const QModelIndex& index);
    void dir_pushButton_clicked();

private:

    std::filesystem::path m_target_dirpath;
    GraphFrame* m_graph_frame;
    DragListView* m_list_view;
    QLabel* m_dir_name;
    QPushButton* m_dir_pushButton;

    void init();
};



