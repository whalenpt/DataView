#include "graphframe.h"
#include "draglistview.h"

#include <QWidget>
#include <QFrame>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QTextEdit>
#include <QMessageBox>
#include <QStackedLayout>

//#include <QDebug>
#include <vector>
#include <ParamBin/parambin.hpp>
#include "fileaux.h"
#include "twocol_m.h"
#include "twocol_log.h"


GraphFrame::GraphFrame(QWidget* parent) : QFrame(parent)
{
    setAcceptDrops(true);

    file_textedit = new QTextEdit(this);
    file_textedit->setReadOnly(true);
    two_col = new TwoCol(this);
    three_col = new ThreeColStacked(this);
    m_two_col = new TwoColM(this);
    m_log_two_col = new TwoColMLog(this);

    frame_layout = new QStackedLayout();
    frame_layout->addWidget(file_textedit);
    frame_layout->addWidget(two_col);
    frame_layout->addWidget(m_two_col);
    frame_layout->addWidget(three_col);
    frame_layout->addWidget(m_log_two_col);
    setLayout(frame_layout);
}

void GraphFrame::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "GraphFrame::dragEnterEvent";
    if(event->mimeData()->hasFormat(DragListView::fileMimeType()))
        event->accept();
    else
        event->ignore();
}

void GraphFrame::dropEvent(QDropEvent* event)
{
    qDebug() << "GraphFrame::dropEvent";
    if(event->mimeData()->hasFormat(DragListView::fileMimeType())){
        QByteArray file_data = event->mimeData()->data(DragListView::fileMimeType());
        QDataStream data_stream(&file_data,QIODevice::ReadOnly);
        QStringList filenames;
        data_stream >> filenames;
        graph(filenames);
    }
}

void GraphFrame::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void GraphFrame::graph(const QStringList& fnames){
    if(fnames.isEmpty())
        return;
    if(fnames.length() == 1)
        graphOneFile(fnames[0].toStdString());
    else
        graphMultiFile(fnames);
}

void GraphFrame::graphMultiFile(const QStringList& filenames)
{
    if(filenames.size() < 2)
        return;

    std::string file_sig;
    std::string fname = filenames[0].toStdString();
    if(!fileSignatureExists(fname))
        fileSignatures[fname] = fileaux::readSignature(fname);
    file_sig = fileSignatures[fname];

    for(int i = 1; i < filenames.size(); i++){
        std::string fname = filenames[i].toStdString();
        if(!fileSignatureExists(fname))
            fileSignatures[fname] = fileaux::readSignature(fname);
        if(file_sig != fileSignatures[fname]){
            QMessageBox::warning(0,"Multiple data file error",
                "Multiple files must have the same data type be analyzed together.");
            return;
        }
    }

    if(file_sig == "two_col_data"){
        m_two_col->graph(filenames);
        frame_layout->setCurrentWidget(m_two_col);
    } else if(file_sig == "two_col_logy_data"){
        m_log_two_col->graph(filenames);
        frame_layout->setCurrentWidget(m_log_two_col);
    }
}

void GraphFrame::graphOneFile(const std::string& fname){
    if(!fileSignatureExists(fname))
        fileSignatures[fname] = fileaux::readSignature(fname);
    std::string file_sig = fileSignatures[fname];
    if(file_sig == "two_col_data"){
        two_col->graph(fname,AxesType::Standard);
        frame_layout->setCurrentWidget(two_col);
    } else if(file_sig == "two_col_logy_data"){
        two_col->graph(fname,AxesType::Semilogy);
        frame_layout->setCurrentWidget(two_col);
    }
    else if(file_sig == "three_col_data"){
        three_col->graph(fname);
        frame_layout->setCurrentWidget(three_col);
    }
    else
        displayFileText(fname);
}

bool GraphFrame::fileSignatureExists(const std::string& fname)
{
    std::map<std::string,std::string>::iterator f_itr = fileSignatures.find(fname);
    if(f_itr != fileSignatures.end())
        return true;
    else
        return false;
}

void GraphFrame::displayFileText(const std::string& fname)
{
    QFile file(QString::fromStdString(fname));
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"info",file.errorString());
    }
    QTextStream infile(&file);
    file_textedit->setText(infile.readAll());
    frame_layout->setCurrentWidget(file_textedit);
}

