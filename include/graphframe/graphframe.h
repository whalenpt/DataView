#ifndef GRAPHFRAME_H
#define GRAPHFRAME_H

#include <QWidget>
#include <QStringList>
#include <QFrame>
#include <string>
#include <pwutils/pwdefs.h>

using DataSignatureMap = std::map<std::string,pw::DataSignature>;
using FileSignatureMap = std::map<std::string,pw::FileSignature>;
using OperatorSignatureMap = std::map<std::string,pw::OperatorSignature>;

class TwoCol;
class ThreeColStacked;
class QTextEdit;
class QStackedLayout;
class QDragEnterEvent;
class QDropEvent;
class QDragMoveEvent;

class GraphFrame : public QFrame
{
    Q_OBJECT
public:
    GraphFrame(QWidget* parent = 0);
    void displayFileText(const QString& fname);

public slots:
    void graphFiles(const QStringList& fnames);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;

private:
    QStackedLayout* m_frame_layout;
    QTextEdit* m_file_textedit;
    TwoCol* m_twocol;
    ThreeColStacked* m_threecol;
    FileSignatureMap m_file_signatures;
    DataSignatureMap m_data_signatures;
    OperatorSignatureMap m_op_signatures;
    bool fileSignatureExists(const QString& fname);
    void graphOneFile(const QString& fname);
    void graphMultipleFiles(const QStringList& fnames);
};

#endif // GRAPHFRAME_H
