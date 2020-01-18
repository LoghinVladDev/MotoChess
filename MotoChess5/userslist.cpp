#include "userslist.h"

#include <QLayout>
#include <QGroupBox>
#include <QPushButton>
UsersList::UsersList(MainWindow* parentApp) : QWidget(parentApp), parent(parentApp)
{
    this->setGeometry(parentApp->geometry());

    QLayout* boxesLayout = new QVBoxLayout(this);

    QGroupBox* usersBox = new QGroupBox(this);

    QGroupBox* buttonsBox = new QGroupBox(this);

    boxesLayout->addWidget(usersBox);
    boxesLayout->addWidget(buttonsBox);

    buttonsBox->setMaximumHeight(100);
    buttonsBox->setMinimumHeight(100);

    QLayout* usersLayout = new QHBoxLayout(usersBox);

    usersTable = new QTableWidget(usersBox);
    usersLayout->addWidget(usersTable);

    usersTable->setColumnCount(2);
    usersTable->setColumnWidth(0,this->usersTable->width()-100);
    usersTable->setColumnWidth(0,100);

    refreshTimer = new QTimer(this);

    refreshTimer->start(1000);

    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    buttonsMapper = new QSignalMapper(this);

    connect(buttonsMapper, SIGNAL(mapped(int)), this, SLOT(challengeUser(int )));
}

void UsersList::refresh()
{
    int usersCount = 0;
    char username[256];

    printf("writing to parent\n");
    fflush(stdout);

    if(0>= write(parent->getSocket(), "refreshUsers", 256))
        return;

    if(0>= read(parent->getSocket(), &usersCount, 4))
        return;

    usersTable->setRowCount(usersCount-1);

    int tableItems = 0;

    for(int i = 0; i < usersCount; i++)
    {
        if(0>=read(parent->getSocket(), username, 256))
            return;

        if(strcmp(username, parent->getUsername().toStdString().c_str()))
        {
            usersTable->setItem(tableItems, 0, new QTableWidgetItem(username));

            QWidget* buttonContainer = new QWidget();
            QLayout* buttonLayout = new QHBoxLayout(buttonContainer);
            QPushButton* challengeButton = new QPushButton();

            buttonLayout->setContentsMargins(0,0,0,0);

            challengeButton->setText("Challenge!");
            buttonLayout->addWidget(challengeButton);

            usersTable->setCellWidget(tableItems,  1, buttonContainer);

            connect(challengeButton, SIGNAL(clicked()), buttonsMapper, SLOT(map()));
            buttonsMapper->setMapping(challengeButton, tableItems++);

        }
    }
    int readValue = 0;

    if( 0>= read(parent->getSocket(),&readValue, 4 ) )
    {
        return;
    }
    if(readValue > 0)
    {
        parent->getPlayerNumber() = readValue;
        refreshTimer->stop();
        emit(pairFound());
    }
}

void UsersList::challengeUser(int whichUser)
{
    printf("%d\n" ,whichUser);
    fflush(stdout);
    printf("Str %s\n", usersTable->item(whichUser,0)->text().toStdString().c_str());
    fflush(stdout);
    write(parent->getSocket(), "challengeUser", 256);
    write(parent->getSocket(), usersTable->item(whichUser, 0)->text().toStdString().c_str(),256);

}
