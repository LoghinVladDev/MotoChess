#include "chesspiece.h"


ChessPiece::ChessPiece(int culoareJuc, int starePiesa)
{
    this->culoarePiesa = culoareJuc;
    this->starePiesa = starePiesa;
}

ChessPiece& ChessPiece::operator=(const ChessPiece &other)
{
    this->starePiesa = other.starePiesa;
    this->piesa->setText(QString(other.piesa->text()));
    this->culoarePiesa = other.culoarePiesa;
    return *this;
}
