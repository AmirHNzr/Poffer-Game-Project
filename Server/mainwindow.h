#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ServerHandler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnStartServer_clicked();
    void NewConnection();
    void NewDC();
    void NewDataSent();
private:
    Ui::MainWindow *ui;
    ServerHandler* _svHandler;
    static int connectionsCount;
};
#endif // MAINWINDOW_H
