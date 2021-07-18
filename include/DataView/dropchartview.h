#ifndef DROPCHARTVIEW_H
#define DROPCHARTVIEW_H

#include <QChartView>
#include <QStringList>

class QDragEnterEvent;
class QDropEvent;
class QDragMoveEvent;
class QChart;
class QWidget;

class DropChartView : public QChartView
{
    Q_OBJECT

public:
    DropChartView(QChart* chart,QWidget* parent=0);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;

signals:
    void fileDrop(const QStringList& filenames);

};

#endif // DROPCHARTVIEW_H

