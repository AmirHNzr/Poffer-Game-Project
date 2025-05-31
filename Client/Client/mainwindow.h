#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include<QStyle>
#include"UserController.h"
#include<QMetaEnum>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include"UserPage.h"
#include"PlayerInfo.h"

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
    void on_lnIP_textChanged(const QString &arg1);

    void on_btnConnect_clicked();


    void UserConnected();
    void UserDisconnected();
    void UserStateChanged(QAbstractSocket::SocketState);
    void UserErrorOccurred(QAbstractSocket::SocketError);


private:
    Ui::MainWindow *ui;
    UserPage* _userPage;
    UserController* _controller;
    PlayerInfo* _player;

    bool permission = false;

    void SetupConnection();
    void SetupLayout();
    void SetupLogRegButton();
    void SetupRegister();
    void SetupLogin();
    void EmailRegex();
};
#endif // MAINWINDOW_H
