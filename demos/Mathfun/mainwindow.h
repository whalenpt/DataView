#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dataview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent* event) override;
private:
    DataView* m_data_view;
    void readSettings();
    void writeSettings();
};
#endif // MAINWINDOW_H
