#include "motochess.h"
#include <QTimer>

MotoChess::MotoChess(MainWindow* parentApp) : QWidget(parentApp), parent(parentApp)
{
    this->setGeometry(parent->geometry());
    this->jucator = parent->getPlayerNumber();
    this->jucator = parent->getPlayerNumber();
    printf("Juc : %d\n", jucator);
        this->nrMiscariPos = 0;
        this->mapper = new QSignalMapper(this);
        connect(this->mapper, SIGNAL(mapped(int)), this, SLOT(click_piesa(int)));
        for(int i = 2; i<6; i++)
            for(int j = 0; j<8; j++)
            {
                this->tabla[i][j] = new ChessPiece(nimic, gol);
            }
        for(int j = 0; j<8; j++)
        {
            this->tabla[1][j] = new ChessPiece(negru, pion);
        }
        this->tabla[0][0] = new ChessPiece(negru, turla);
        this->tabla[0][1] = new ChessPiece(negru, cavaler);
        this->tabla[0][2] = new ChessPiece(negru, nebun);
        this->tabla[0][3] = new ChessPiece(negru, regina);
        this->tabla[0][4] = new ChessPiece(negru, rege);
        this->tabla[0][5] = new ChessPiece(negru, nebun);
        this->tabla[0][6] = new ChessPiece(negru, cavaler);
        this->tabla[0][7] = new ChessPiece(negru, turla);
        for(int j = 0; j<8; j++)
        {
            this->tabla[6][j] = new ChessPiece(alb, pion);
        }
        this->tabla[7][0] = new ChessPiece(alb, turla);
        this->tabla[7][1] = new ChessPiece(alb, cavaler);
        this->tabla[7][2] = new ChessPiece(alb, nebun);
        this->tabla[7][3] = new ChessPiece(alb, regina);
        this->tabla[7][4] = new ChessPiece(alb, rege);
        this->tabla[7][5] = new ChessPiece(alb, nebun);
        this->tabla[7][6] = new ChessPiece(alb, cavaler);
        this->tabla[7][7] = new ChessPiece(alb, turla);
        QString piesa;
            tableWidget = new QWidget(this);
            tableWidget->hide();
            QGridLayout *layoutPiese = new QGridLayout(tableWidget);
            tableWidget->resize(800,800);
            tableWidget->move(0,0);
            for(int i = 0; i< 8; i++)
            {
                for(int j = 0; j< 8; j++)
                {
                    if(this->tabla[i][j]->GetStarePiesa() == gol)
                        piesa = "nimic";
                    if(this->tabla[i][j]->GetStarePiesa() == pion)
                        piesa = "pion";
                    if(this->tabla[i][j]->GetStarePiesa() == cavaler)
                        piesa = "cavaler";
                    if(this->tabla[i][j]->GetStarePiesa() == turla)
                        piesa = "turla";
                    if(this->tabla[i][j]->GetStarePiesa() == nebun)
                        piesa = "nebun";
                    if(this->tabla[i][j]->GetStarePiesa() == regina)
                        piesa = "regina";
                    if(this->tabla[i][j]->GetStarePiesa() == rege)
                        piesa = "rege";
                    this->tabla[i][j]->piesa = new QPushButton(tableWidget);
                    this->tabla[i][j]->piesa->setFlat(false);

                    this->tabla[i][j]->piesa->setFixedSize(QSize(75,75));
                    this->tabla[i][j]->piesa->update();
                    this->tabla[i][j]->piesa->setText(piesa);
                    layoutPiese->addWidget(this->tabla[i][j]->piesa, i, j );
                    connect(this->tabla[i][j]->piesa, SIGNAL(clicked()), this->mapper, SLOT(map()));
                    this->mapper->setMapping(this->tabla[i][j]->piesa, i*8 + j);
                }
            }

      refreshTimer = new QTimer();
      refreshTimer->start(1200);
      connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
      tableWidget->show();
}

void MotoChess::refresh()
{
    write(parent->getSocket(), "refreshTurns", 256);
    read(parent->getSocket(), &playerTurn, 4);
}



bool isInTable(int y, int x)
{

    if(x >= 8 || y >= 8 || x < 0 || y < 0)
        return false;
    return true;
}

int MotoChess::isInMoves(int y, int x)
{
    for(int i = 1; i<this->nrMiscariPos; i++)
    {
        if(y == this->miscari[i].y && x == this->miscari[i].x)
            return i;
    }
    return -1;
}

void MotoChess::click_piesa(int id)
{
    int table[8][8];
    char messageBuffer[64];
    int playerTurn = 1;
    int calX[] = {1, 2, 2, 1, -1, -2, -2, -1};
    int calY[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    int regeX[] = {0, 1, 1, 1, 0, -1, -1, -1};
    int regeY[] = {-1, -1, 0, 1, 1, 1, 0, -1};


    printf("%d\n", id);
    int xSelCur = id % 8;
    int ySelCur = id / 8;



    for(int i = 1; i<nrMiscariPos; i++)
    {
        this->tabla[this->miscari[i].y][this->miscari[i].x]->piesa->setFlat(false);
        if(this->tabla[this->miscari[i].y][this->miscari[i].x]->piesa->text().contains(QString("Atac ")))
        {
            switch(this->tabla[this->miscari[i].y][this->miscari[i].x]->GetStarePiesa())
            {
                case pion:
                    this->tabla[this->miscari[i].y][this->miscari[i].x]->piesa->setText(QString("pion"));
                    break;
                case turla:
                    this->tabla[this->miscari[i].y][this->miscari[i].x]->piesa->setText(QString("turla"));
                    break;
                case cavaler:
                    this->tabla[this->miscari[i].y][this->miscari[i].x]->piesa->setText(QString("cavaler"));
                    break;
                case nebun:
                    this->tabla[this->miscari[i].y][this->miscari[i].x]->piesa->setText(QString("nebun"));
                    break;
                case rege:
                    this->tabla[this->miscari[i].y][this->miscari[i].x]->piesa->setText(QString("rege"));
                    break;
                case regina:
                    this->tabla[this->miscari[i].y][this->miscari[i].x]->piesa->setText(QString("regina"));
                    break;
            }
        }
    }
    //this->update();
    int move = 0;

    if((move = this->isInMoves(ySelCur, xSelCur))>0)
    {
        strcpy(messageBuffer, "GETSTATE");
        //write(sockDesc, messageBuffer, 64);
        //read(sockDesc, &playerTurn, 4);
        if(playerTurn == jucator)
        {
            if(playerTurn == 1)
                playerTurn=2;
            else
                playerTurn=1;
            strcpy(messageBuffer, "APPLYMOVE");
            //write(sockDesc, messageBuffer, 64);
            if(this->miscari[move].tip == tranzitie)
            {
                ChessPiece aux;
                aux.piesa = new QPushButton();
                aux = *this->tabla[ySelCur][xSelCur];
                *this->tabla[ySelCur][xSelCur] = *this->tabla[this->miscari[0].y][this->miscari[0].x];
                *this->tabla[this->miscari[0].y][this->miscari[0].x] = aux;


            }
            if(this->miscari[move].tip == atac)
            {
                *this->tabla[ySelCur][xSelCur] = *this->tabla[this->miscari[0].y][this->miscari[0].x];
                this->tabla[this->miscari[0].y][this->miscari[0].x]->piesa->setText(QString("nimic"));
                this->tabla[this->miscari[0].y][this->miscari[0].x]->SetCuloarePiesa(nimic);
                this->tabla[this->miscari[0].y][this->miscari[0].x]->SetStarePiesa(gol);
            }
            for (int i = 0; i< 8; i++) {
                for (int j=0;j<8;j++) {
                    table[i][j] = 0;
                    if(this->tabla[i][j]->GetStarePiesa())
                    {
                        table[i][j] = tabla[i][j]->GetStarePiesa() + (this->jucator-1)*6;
                    }
                }
            }
            //write(sockDesc, table, sizeof(table));
        }
        this->nrMiscariPos = 0;
        memset(this->miscari, 0 ,sizeof(this->miscari));
        return;
    }
    if(this->tabla[ySelCur][xSelCur]->GetStarePiesa() != gol)
    {
        if(this->tabla[ySelCur][xSelCur]->GetCuloarePiesa() != this->jucator)
            return;
        this->nrMiscariPos = 0;
        memset(this->miscari, 0, sizeof(this->miscari));
        if(this->tabla[ySelCur][xSelCur]->GetStarePiesa() == pion)
        {
            this->miscari[nrMiscariPos].tip = piesa;
            this->miscari[nrMiscariPos].y = ySelCur;
            this->miscari[nrMiscariPos++].x = xSelCur;

            int dir = 0;
            if(this->jucator == 1)
                dir = -1;
            if(this->jucator == 2)
                dir = 1;
            if(ySelCur == 1 || ySelCur == 6) // se poate misca 2
            {
                if(isInTable(ySelCur + 2*dir, xSelCur))
                {
                    if(this->tabla[ySelCur + dir][xSelCur]->GetStarePiesa() == gol && this->tabla[ySelCur + dir + dir][xSelCur]->GetStarePiesa()==gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].y = ySelCur + 2*dir;
                        this->miscari[nrMiscariPos++].x = xSelCur;
                    }
                }
            }
            if(isInTable(ySelCur+dir, xSelCur)) // se poate misca 1
                if(this->tabla[ySelCur + dir][xSelCur]->GetStarePiesa()==gol)
                {
                    this->miscari[nrMiscariPos].tip = tranzitie;
                    this->miscari[nrMiscariPos].y = ySelCur + dir;
                    this->miscari[nrMiscariPos++].x = xSelCur;
                }
            for(int dirHor = -1; dirHor <= 1; dirHor+=2)
            {                                                             // poate ataca
                if(isInTable(ySelCur + dir, xSelCur + dirHor))
                {
                    if(this->tabla[ySelCur + dir][xSelCur + dirHor]->GetStarePiesa()!=gol &&
                       this->tabla[ySelCur + dir][xSelCur + dirHor]->GetCuloarePiesa() != this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].y = ySelCur + dir;
                        this->miscari[nrMiscariPos++].x = xSelCur + dirHor;
                    }
                }
            }
        }
        if(this->tabla[ySelCur][xSelCur]->GetStarePiesa() == cavaler)
        {


            this->miscari[nrMiscariPos].tip = piesa;
            this->miscari[nrMiscariPos].y = ySelCur;
            this->miscari[nrMiscariPos++].x = xSelCur;
            int ln, cn;
            for(int dir = 0; dir < 8; dir++)
            {
                ln = ySelCur + calY[dir];
                cn = xSelCur + calX[dir];
                if(isInTable(ln, cn))
                {
                    if(this->tabla[ln][cn]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = cn;
                        this->miscari[nrMiscariPos++].y = ln;
                    }
                    if(this->tabla[ln][cn]->GetStarePiesa() != gol && this->tabla[ln][cn]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = cn;
                        this->miscari[nrMiscariPos++].y = ln;
                    }
                }
            }
        }
        if(this->tabla[ySelCur][xSelCur]->GetStarePiesa() == turla)
        {
            this->miscari[nrMiscariPos].tip = piesa;
            this->miscari[nrMiscariPos].y = ySelCur;
            this->miscari[nrMiscariPos++].x = xSelCur;
            int x,y;
            for(x = xSelCur-1, y = ySelCur; x >= 0; x--)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur+1, y = ySelCur; x < 8; x++)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur, y = ySelCur-1; y >= 0; y--)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur, y = ySelCur+1; y < 8; y++)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
        }
        if(this->tabla[ySelCur][xSelCur]->GetStarePiesa() == nebun)
        {
            this->miscari[nrMiscariPos].tip = piesa;
            this->miscari[nrMiscariPos].y = ySelCur;
            this->miscari[nrMiscariPos++].x = xSelCur;
            int x, y;
            for(x = xSelCur + 1, y = ySelCur + 1; x < 8 && y < 8; x++,y++)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur + 1, y = ySelCur - 1; x < 8 && y >= 0; x++,y--)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur - 1, y = ySelCur + 1; x >= 0 && y < 8; x--,y++)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur - 1, y = ySelCur - 1; x >= 0 && y >= 0; x--,y--)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
        }
        if(this->tabla[ySelCur][xSelCur]->GetStarePiesa() == regina)
        {
            this->miscari[nrMiscariPos].tip = piesa;
            this->miscari[nrMiscariPos].y = ySelCur;
            this->miscari[nrMiscariPos++].x = xSelCur;
            int x,y;
            for(x = xSelCur-1, y = ySelCur; x >= 0; x--)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur+1, y = ySelCur; x < 8; x++)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur, y = ySelCur-1; y >= 0; y--)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur, y = ySelCur+1; y < 8; y++)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }

            for(x = xSelCur + 1, y = ySelCur + 1; x < 8 && y < 8; x++,y++)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur + 1, y = ySelCur - 1; x < 8 && y >= 0; x++,y--)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur - 1, y = ySelCur + 1; x >= 0 && y < 8; x--,y++)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
            for(x = xSelCur - 1, y = ySelCur - 1; x >= 0 && y >= 0; x--,y--)
            {
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                        break;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()== this->jucator)
                        break;
                }
            }
        }
        if(this->tabla[ySelCur][xSelCur]->GetStarePiesa()==rege)
        {
            this->miscari[nrMiscariPos].tip = piesa;
            this->miscari[nrMiscariPos].y = ySelCur;
            this->miscari[nrMiscariPos++].x = xSelCur;
            int y, x;
            for(int dir = 0; dir < 8; dir++)
            {
                y = ySelCur + regeY[dir];
                x = xSelCur + regeX[dir];
                if(isInTable(y, x))
                {
                    if(this->tabla[y][x]->GetStarePiesa() == gol)
                    {
                        this->miscari[nrMiscariPos].tip = tranzitie;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                    if(this->tabla[y][x]->GetStarePiesa() != gol && this->tabla[y][x]->GetCuloarePiesa()!= this->jucator)
                    {
                        this->miscari[nrMiscariPos].tip = atac;
                        this->miscari[nrMiscariPos].x = x;
                        this->miscari[nrMiscariPos++].y = y;
                    }
                }
            }
        }
    }

    for(int i = 1; i<nrMiscariPos; i++)
    {
        this->tabla[this->miscari[i].y][this->miscari[i].x]->piesa->setFlat(true);
        if(miscari[i].tip == atac)
            this->tabla[this->miscari[i].y][this->miscari[i].x]->piesa->setText(QString("Atac ") + this->tabla[this->miscari[i].y][this->miscari[i].x]->piesa->text());
    }
    fflush(stdout);
}
