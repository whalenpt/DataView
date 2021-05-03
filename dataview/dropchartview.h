#ifndef DROPCHARTVIEW_H
#define DROPCHARTVIEW_H

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QStringList>

class DropChartView : public QtCharts::QChartView
{
    Q_OBJECT

public:
    DropChartView(QtCharts::QChart* chart,QWidget* parent=0);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;

signals:
    //void fileDrop(const std::string& filename);
    void fileDrop(const QStringList& filenames);

};

#endif // DROPCHARTVIEW_H

