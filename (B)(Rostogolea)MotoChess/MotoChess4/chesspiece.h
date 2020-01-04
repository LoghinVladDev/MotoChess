#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QLabel>

#include <QObject>
#include <QPushButton>

enum stariPiese
{
    gol, pion, cavaler, nebun, turla, regina, rege
};

enum culoriJuc
{
    nimic, alb, negru
};

enum tipMutare
{
    tranzitie, atac, piesa
};

class ChessPiece
{
private:
    int starePiesa;
    int culoarePiesa;
    int culoareCareu;
public:
    ChessPiece() {}
    ChessPiece(int culoareJuc, int starePiesa);
    ChessPiece& operator= (const ChessPiece& other);
    QPushButton *piesa;
    inline int GetStarePiesa() { return this->starePiesa; }
    inline void SetStarePiesa(int stareOther) {this->starePiesa = stareOther; }
    inline int GetCuloarePiesa() { return this->culoarePiesa; }
    inline void SetCuloarePiesa(int culoareOther) {this->culoarePiesa = culoareOther; }
};

#endif // CHESSPIECE_H
