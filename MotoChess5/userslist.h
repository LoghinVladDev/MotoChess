#ifndef USERSLIST_H
#define USERSLIST_H

#include <QWidget>
#include "mainwindow.h"
#include <QTimer>
#include <QTableWidget>
#include <QSignalMapper>

class UsersList: public QWidget
{
    Q_OBJECT
public:
    UsersList(MainWindow*);
signals:
    void pairFound();
private slots:
    void refresh();
    void challengeUser(int);
private:
    MainWindow* parent;
    QTableWidget* usersTable;
    QSignalMapper* buttonsMapper;
    QTimer* refreshTimer;
};

#endif // USERSLIST_H
