
#include <QWidget>
#include <QFileDialog>  
#include <QFile>
#include <QString>
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include <ParamBin/parambin.hpp>
#include "dataview.h"
#include "draglistview.h"
#include "graphframe/graphframe.h"

DataView::DataView(QWidget *parent) :
    QDialog(parent),
    m_target_dirpath(std::filesystem::path(QDir::homePath().toStdString())),
    m_graph_frame(new GraphFrame),
    m_list_view(new DragListView(m_target_dirpath)),
    m_dir_name(new QLabel("Placeholder")),
    m_dir_pushButton(new QPushButton("Choose..."))
{
    setTargetDirPath(m_target_dirpath);
    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel("Directory Name:"));
    hbox->addWidget(m_dir_name);
    hbox->addStretch();
    hbox->addWidget(m_dir_pushButton);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addWidget(new QLabel("Files:"));
    vbox->addWidget(m_list_view);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addLayout(vbox);
    layout->addWidget(m_graph_frame);

    init();
    setLayout(layout);
    setWindowTitle("Data Analyzer");
}

DataView::~DataView() { }

void DataView::setTargetDirPath(const std::filesystem::path& dirpath) {
    m_target_dirpath = dirpath;
    m_dir_name->setText(QString::fromStdString(dirpath.string()));
    m_list_view->setTargetDirPath(dirpath);
}

void DataView::init()
{
    connect(m_list_view,&DragListView::doubleClicked,this,&DataView::doubleClickedResponse);
    m_dir_pushButton->setFocusPolicy(Qt::NoFocus);
    connect(m_dir_pushButton,&QPushButton::clicked,this,&DataView::dir_pushButton_clicked);
}

void DataView::doubleClickedResponse(const QModelIndex& index){
    if(!m_list_view->isDir(index)){
        QStringList filelist;
        m_list_view->getFileNames(filelist);
        m_graph_frame->graphFiles(filelist);
    }
    return;
}

void DataView::dir_pushButton_clicked()
{
    QString dir_name = QDir::homePath();
    if(std::filesystem::is_directory(m_target_dirpath))
        dir_name = QString::fromStdString(m_target_dirpath.string());
    dir_name = QFileDialog::getExistingDirectory(this,"Open file directory",
                                                 dir_name);
    if(!dir_name.isEmpty())
        setTargetDirPath(std::filesystem::path(dir_name.toStdString()));
}




