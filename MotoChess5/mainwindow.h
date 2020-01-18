#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void InitLogin();
    void ConnectToServer();
    int& getConnectionStatus() { return isConnected; }
    ~MainWindow();

    int & getSocket() { return socketDescriptor; }
    int & getPlayerNumber() { return playerNumber; }

    QString& getUsername() { return username; }
private slots:
    void InitUsersList();
    void InitGame();
private:
    int isConnected{0};
    int playerNumber = 0;
    int socketDescriptor;
    QString username;
    Ui::MainWindow *ui;
    sockaddr_in serverInformation;
};
#endif // MAINWINDOW_H
