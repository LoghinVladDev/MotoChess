#ifndef MOTOCHESS_H
#define MOTOCHESS_H


#include <QWidget>

struct miscare
{
    int x, y, tip;
};

#include "mainwindow.h"
#include "chesspiece.h"
#include <QSignalMapper>
#include <QPushButton>
#include <QGridLayout>
class MotoChess : public QWidget
{
    Q_OBJECT
public:
    MotoChess(MainWindow*);
    int isInMoves(int ,int);
private:
    MainWindow* parent;
    QTimer* update;
    int jucator;
    int nrMiscariPos;
    int playerTurn;
    ChessPiece* tabla[8][8];
    QSignalMapper* mapper;
    QWidget *tableWidget;
    QTimer* refreshTimer;
    miscare miscari[64];
private slots:
    void refresh();
    void click_piesa(int id);

};

#endif // MOTOCHESS_H
