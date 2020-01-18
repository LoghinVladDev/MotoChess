#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwidget.h"
#include "motochess.h"
#include "userslist.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    this->window()->resize(1280, 720);

    serverInformation.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverInformation.sin_port = htons(3500);
    serverInformation.sin_family = AF_INET;

    socketDescriptor = 0;

    InitLogin();
}

void MainWindow::InitLogin()
{
    LoginWidget* login = new LoginWidget(this);

    this->setCentralWidget(login);

    connect(login, SIGNAL(connected()), this, SLOT(InitUsersList()));
}

void MainWindow::InitUsersList()
{
    this->centralWidget()->hide();

    UsersList* users = new UsersList(this);

    this->setCentralWidget(users);

    connect(users, SIGNAL(pairFound()), this,SLOT(InitGame()) );
}


void MainWindow::InitGame()
{
    MotoChess* game = new MotoChess(this);

    this->setCentralWidget(game);

}


void MainWindow::ConnectToServer()
{
    if(socketDescriptor != 0)
        ::close(socketDescriptor);

    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    if(socketDescriptor == -1)
    {
        printf("Socket failed\n");
        fflush(stdout);
        return;
    }

    if( -1 == ::connect(socketDescriptor, (sockaddr*)&serverInformation, sizeof(sockaddr)))
    {
        printf("Connection Refused\n");
        fflush(stdout);
        return;
    }
    getConnectionStatus() = 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

