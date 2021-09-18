// dropchartview
#pragma once

#include <QChartView>
#include <QStringList>

class QDragEnterEvent;
class QDropEvent;
class QDragMoveEvent;
class QChart;
class QWidget;
class GraphFrame;

class DropChartView : public QChartView
{
    Q_OBJECT

public:
    DropChartView(QChart* chart,GraphFrame* graph_frame);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;

signals:
    void fileListDrop(const QStringList& filenames);

private:
    GraphFrame* m_graph_frame;

};


