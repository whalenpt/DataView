#include "mainwindow.h"
#include "dataview.h"
#include <QVBoxLayout>
#include <QSettings>
#include <QCloseEvent>
#include <QString>
#include <QDebug>

const QString WINDOWSIZE_SETTING("MainWindow/size");
const QString WINDOWPOS_SETTING("MainWindow/pos");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_data_view(new DataView)
{
    m_data_view->setTargetPath("/Users/pmacbook/Projects/Qt/UPPE/tests/ArgonShock");
    connect(m_data_view,&DataView::accepted,this,&MainWindow::writeSettings);
    readSettings();
    setCentralWidget(m_data_view);
//    qDebug() << "Set DataView to main window.";
}
void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    event->accept();
}

void MainWindow::readSettings()
{
    QSettings settings("Mathfun");
    this->resize(settings.value(WINDOWSIZE_SETTING,QSize(1000,500)).toSize());
    this->move(settings.value(WINDOWPOS_SETTING,QPoint(300,300)).toPoint());
}

void MainWindow::writeSettings()
{
    QSettings settings("Mathfun");
    settings.setValue(WINDOWSIZE_SETTING,size());
    settings.setValue(WINDOWPOS_SETTING,pos());
}


MainWindow::~MainWindow()
{
}

