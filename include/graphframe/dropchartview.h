// dropwidget.h
#pragma once

#include <QString>
#include <QStringList>
#include <QWidget>
#include <QFrame>
#include <QChart>
#include <QChartView>

class QDragEnterEvent;
class QDropEvent;
class QDragMoveEvent;

class DropChartView : public QChartView
{
    Q_OBJECT

public:
    explicit DropChartView(QChart* chart,QWidget* parent_widget);
    ~DropChartView() {};

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;

signals:
    void fileDrop(const QStringList& filenames);
};

class DropWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DropWidget(QWidget* parent_widget);
    ~DropWidget() {};

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;

signals:
    void fileDrop(const QStringList& filenames);
};


class DropFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DropFrame(QWidget* parent);
    ~DropFrame() {};

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;

signals:
    void fileDrop(const QStringList& filenames);

};


