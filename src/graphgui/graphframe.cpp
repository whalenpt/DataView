
#include "graphgui/graphframe.h"
#include "graphgui/graphtype/twocol_m.h"
#include "graphgui/graphtype/twocol.h"
#include "graphgui/graphtype/threecolstacked.h"
#include "listgui/draglistview.h"
#include "core/fileaux.h"

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
        m_twocolM->graph(filenames,AxesType::Standard);
        m_frame_layout->setCurrentWidget(m_twocolM);
    } else if(file_sig == "two_col_logy_data"){
        m_twocolM->graph(filenames,AxesType::Semilogy);
        m_frame_layout->setCurrentWidget(m_twocolM);
    }
}

void GraphFrame::graphOneFile(const std::string& fname){
    if(!fileSignatureExists(fname))
        fileSignatures[fname] = fileaux::readSignature(fname);
    std::string file_sig = fileSignatures[fname];
    if(file_sig == "two_col_data"){
        m_twocol->graph(fname,AxesType::Standard);
        m_frame_layout->setCurrentWidget(m_twocol);
    } else if(file_sig == "two_col_logy_data"){
        m_twocol->graph(fname,AxesType::Semilogy);
        m_frame_layout->setCurrentWidget(m_twocol);
    }
    else if(file_sig == "three_col_data"){
        m_threecol->graph(fname);
        m_frame_layout->setCurrentWidget(m_threecol);
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
    m_file_textedit->setText(infile.readAll());
    m_frame_layout->setCurrentWidget(m_file_textedit);
}

