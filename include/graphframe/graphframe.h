// graphframe.h
#pragma once

#include <QWidget>
#include <QStringList>
#include <QFrame>
#include <string>
#include <pwutils/pwdefs.h>
#include "graphframe/dropchartview.h"

using DataSignatureMap = std::map<std::string,pw::DataSignature>;
using FileSignatureMap = std::map<std::string,pw::FileSignature>;
using OperatorSignatureMap = std::map<std::string,pw::OperatorSignature>;

class TwoCol;
class ThreeColStacked;
class SurfaceWidget;
class QTextEdit;
class QStackedLayout;

class GraphFrame : public DropFrame
{
    Q_OBJECT
public:
    GraphFrame(QWidget* parent = 0);
    void displayFileText(const QString& fname);
    void setMaxPoint1D(unsigned int points);
    void setMaxPoint2DX(unsigned int points); 
    void setMaxPoint2DY(unsigned int points);

public slots:
    void graphFiles(const QStringList& fnames);

private:
    QStackedLayout* m_frame_layout;
    QTextEdit* m_file_textedit;
    TwoCol* m_twocol;
    ThreeColStacked* m_threecol;
    SurfaceWidget* m_surface;
    FileSignatureMap m_file_signatures;
    DataSignatureMap m_data_signatures;
    OperatorSignatureMap m_op_signatures;
    bool fileSignatureExists(const QString& fname);
    void graphOneFile(const QString& fname);
    void graphMultipleFiles(const QStringList& fnames);
};

