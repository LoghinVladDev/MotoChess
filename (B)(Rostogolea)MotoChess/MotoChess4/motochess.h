#ifndef MOTOCHESS_H
#define MOTOCHESS_H

#include <QMainWindow>
#include <QSignalMapper>
#include "chesspiece.h"
#include "string.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "sys/stat.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <QLineEdit>
#include <unistd.h>
#include <QGroupBox>
#include <QTableWidget>
#include <QTimer>
#include <QCloseEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class MotoChess; }
QT_END_NAMESPACE


struct miscare
{
    int x, y, tip;
};

class MotoChess : public QMainWindow
{
    Q_OBJECT

public:
    MotoChess(QWidget *parent = nullptr);
    ~MotoChess();
    void Matchmaking();
    int isInMoves(int y, int x);

    void treatChallenge();

    void closeEvent(QCloseEvent* event);

    void AsteaptaUpdate();
private slots:

    void refreshUsersList();

    void buttonMatch_clicked(QString);

    void refresh_table();

    void click_piesa(int id);

    void loginClicked();

private:
    QGroupBox* loginGroup;
    QTableWidget* usersTable;
    QLineEdit* lineEditUser;
    QWidget* tableWidget;
    QWidget* matchmakingWidget;
    QTimer* timerRefresh;
    QTimer* tableRefresh;
    sockaddr_in serverInfo;

    int sockDesc;
    int challenge{0};
    int isChallenger{0};
    int jucator;
    int tura{0};

    int nrMiscariPos;

    miscare miscari[64];

    ChessPiece *tabla[8][8];

    QSignalMapper *mapper;
    QSignalMapper *matchmakingMapper;

    Ui::MotoChess *ui;
};
#endif // MOTOCHESS_H
