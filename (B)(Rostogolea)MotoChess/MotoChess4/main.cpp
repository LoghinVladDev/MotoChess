#include "motochess.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MotoChess w;
    w.setGeometry(0,0,1920,1080);
    w.resize(1920, 1080);
    w.update();
    w.show();
    return a.exec();
}
