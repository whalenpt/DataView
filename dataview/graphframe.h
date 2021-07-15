#ifndef GRAPHFRAME_H
#define GRAPHFRAME_H

#include <QWidget>
#include <QStringList>
#include <QFrame>
#include <string>
#include "twocol.h"
#include "threecolstacked.h"
#include "twocol_m.h"

using StrMap = std::map<std::string,std::string>;

class TwoCol;
class ThreeColStacked;
class TwoColM;
class TwoColMLog;
class QTextEdit;
class QStackedLayout;

class GraphFrame : public QFrame
{
    Q_OBJECT
public:
    GraphFrame(QWidget* parent = 0);
    void displayFileText(const std::string& fname);

public slots:
    void graph(const QStringList& fnames);
    void graphOneFile(const std::string& fname);
    void graphMultiFile(const QStringList& fnames);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;

private:
    QStackedLayout* m_frame_layout;
    QTextEdit* m_file_textedit;
    TwoCol* m_twocol;
    ThreeColStacked* m_threecol;
    TwoColM* m_twocolM;

    StrMap fileSignatures;
    bool fileSignatureExists(const std::string& fname);
};

#endif // GRAPHFRAME_H
