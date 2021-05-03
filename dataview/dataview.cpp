
#include <QtWidgets>
#include <QFile>
#include <QString>
#include <map>
#include <string>
#include <vector>
#include <parambin.hpp>
#include "draglistview.h"
#include "dataview.h"
#include "graphframe.h"

DataView::DataView(QWidget *parent) :
    QDialog(parent)
{
    target_path = QDir::homePath();
    list_view = new DragListView(target_path);
    graph_frame = new GraphFrame();
    dir_name = new QLabel("Placeholder");
    dir_pushButton = new QPushButton("Choose...");
    setTargetPath(target_path);

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel("Directory Name:"));
    hbox->addWidget(dir_name);
    hbox->addStretch();
    hbox->addWidget(dir_pushButton);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addWidget(new QLabel("Files:"));
    vbox->addWidget(list_view);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addLayout(vbox);
    layout->addWidget(graph_frame);

    init();
    setLayout(layout);
    setWindowTitle("Data Analyzer");
}

DataView::~DataView()
{
}

void DataView::setTargetPath(const QString& c_target_path) {
    target_path = c_target_path;
    QDir dir(c_target_path);
    dir_name->setText(dir.dirName());
    list_view->setTargetPath(c_target_path);
}

//void DataView::closeEvent(QCloseEvent *event)
//{
//    writeSettings();
//    event->accept();
//}
//
//void DataView::writeSettings()
//{
//    QSettings settings("pw", "DataView");
//    settings.setValue("Geometry",saveGeometry());
//}
//
//void DataView::readSettings()
//{
//    QSettings settings("pw", "DataView");
//    restoreGeometry(settings.value("Geometry").toByteArray());
//}

void DataView::init()
{
    connect(list_view,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(graphIfFile(QModelIndex)));
    dir_pushButton->setFocusPolicy(Qt::NoFocus);
    connect(dir_pushButton,SIGNAL(clicked(bool)),
            this,SLOT(dir_pushButton_clicked()));
}

void DataView::graphIfFile(QModelIndex index){
    if(!list_view->isDir(index)){
        QStringList slist;
        list_view->getFileNames(slist);
//        qDebug() << "DataView::graphIfFile: " << slist;
        std::string fname = slist[0].toStdString();
        graph_frame->graphOneFile(fname);
    }
    return;
}

void DataView::dir_pushButton_clicked()
{
    QString dir_name = target_path;
    if(dir_name.isEmpty())
        dir_name = QDir::homePath() + QDir::separator();
    dir_name = QFileDialog::getExistingDirectory(this,"Open file directory",
                                                 dir_name);
    if(!dir_name.isEmpty())
        setTargetPath(dir_name+QDir::separator());
}




