
#include "graphframe/graphframe.h"
#include "graphframe/twocol_m.h"
#include "graphframe/twocol.h"
#include "graphframe/threecolstacked.h"
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

#include <vector>
#include <ParamBin/parambin.hpp>
#include <pwutils/pwdefs.h>
#include <pwutils/read/readfile.h>


GraphFrame::GraphFrame(QWidget* parent) : QFrame(parent)
{
    setAcceptDrops(true);

    m_file_textedit = new QTextEdit(this);
    m_file_textedit->setReadOnly(true);

    m_twocol = new TwoCol(this);
    m_threecol = new ThreeColStacked(this);
    m_twocolM = new TwoColM(this);

    m_frame_layout = new QStackedLayout();
    m_frame_layout->addWidget(m_file_textedit);
    m_frame_layout->addWidget(m_twocol);
    m_frame_layout->addWidget(m_twocolM);
    m_frame_layout->addWidget(m_threecol);
    setLayout(m_frame_layout);
}

void GraphFrame::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText() || \
            event->mimeData()->hasFormat(DragListView::StringListMime)){
        event->accept();
    }
    else
        event->ignore();
}

void GraphFrame::dropEvent(QDropEvent* event)
{
    if(event->mimeData()->hasText())
        graphOneFile(event->mimeData()->text());
    else if(event->mimeData()->hasFormat(DragListView::StringListMime)){
        QByteArray file_data = event->mimeData()->data(DragListView::StringListMime);
        QDataStream data_stream(&file_data,QIODevice::ReadOnly);
        QStringList filelist;
        data_stream >> filelist;
        graphMultipleFiles(filelist);
    }
}

void GraphFrame::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void GraphFrame::graphFiles(const QStringList& filenames)
{
    if(filenames.empty())
        return;
    if(filenames.size() == 1)
        graphOneFile(filenames.front());
    else
        graphMultipleFiles(filenames);
}

void GraphFrame::graphMultipleFiles(const QStringList& filenames)
{
    std::string fname(filenames[0].toStdString());
    if(!fileSignatureExists(filenames[0])){
        m_file_signatures[fname] = pw::fileSignature(fname);
        m_data_signatures[fname] = pw::dataSignature(fname,m_file_signatures[fname]);
        m_op_signatures[fname] = pw::operatorSignature(fname,m_file_signatures[fname]);
    }
    pw::FileSignature file_sig = m_file_signatures[fname];
    pw::DataSignature data_sig = m_data_signatures[fname];
    pw::OperatorSignature op_sig = m_op_signatures[fname];

    for(auto i = 1; i < filenames.size(); i++){
        std::string fname(filenames[i].toStdString());
        if(!fileSignatureExists(filenames[i])){
            m_file_signatures[fname] = pw::fileSignature(fname);
            m_data_signatures[fname] = pw::dataSignature(fname,m_file_signatures[fname]);
            m_op_signatures[fname] = pw::operatorSignature(fname,m_file_signatures[fname]);
        }

        if(m_file_signatures[fname] != file_sig){
            QMessageBox::warning(0,"Multiple data file error",
                "Multiple files must have the same file signature be analyzed together.");
            return;
        }
        if(m_data_signatures[fname] != data_sig){
            QMessageBox::warning(0,"Multiple data file error",
                "Multiple files must have the same data signature be analyzed together.");
            return;
        }
        if(m_op_signatures[fname] != op_sig){
            QMessageBox::warning(0,"Multiple data file error",
                "Multiple files must have the same operator signature be analyzed together.");
            return;
        }
    }
    if(data_sig == pw::DataSignature::XY){
        m_twocolM->graph(filenames,file_sig,data_sig,op_sig);
        m_frame_layout->setCurrentWidget(m_twocolM);
    }
    else{
        QMessageBox::warning(0,"Multiple data file error",
                    "Multiple files must have the same file signature be analyzed together.");
                return;
        displayFileText(filenames[0]);
    }
}

void GraphFrame::graphOneFile(const QString& filename){

    std::string fname(filename.toStdString());
    if(!fileSignatureExists(filename)){
        m_file_signatures[fname] = pw::fileSignature(fname);
        m_data_signatures[fname] = pw::dataSignature(fname,m_file_signatures[fname]);
        m_op_signatures[fname] = pw::operatorSignature(fname,m_file_signatures[fname]);
    }
    pw::FileSignature file_sig = m_file_signatures[fname];
    pw::DataSignature data_sig = m_data_signatures[fname];
    pw::OperatorSignature op_sig = m_op_signatures[fname];

    if(data_sig == pw::DataSignature::XY){
        m_twocol->graph(filename,file_sig,data_sig,op_sig);
        m_frame_layout->setCurrentWidget(m_twocol);
    }
    else if(data_sig == pw::DataSignature::XY_C){
        m_threecol->graph(filename,file_sig,data_sig,op_sig);
        m_frame_layout->setCurrentWidget(m_threecol);
    }
    else{
        displayFileText(filename);
    }
}

bool GraphFrame::fileSignatureExists(const QString& fname)
{
    const auto f_itr = m_file_signatures.find(fname.toStdString());
    return (f_itr != m_file_signatures.end()) ? true : false;
}

void GraphFrame::displayFileText(const QString& fname)
{
    QFile file(fname);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"info",file.errorString());
    }
    QTextStream infile(&file);
    m_file_textedit->setText(infile.readAll());
    m_frame_layout->setCurrentWidget(m_file_textedit);
}

