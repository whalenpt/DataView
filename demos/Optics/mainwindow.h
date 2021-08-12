#ifndef OPTICSMAINWINDOW_H
#define OPTICSMAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <DataView>

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
#endif // OPTICSMAINWINDOW_H
