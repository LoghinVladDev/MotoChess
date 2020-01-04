/********************************************************************************
** Form generated from reading UI file 'motochess.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOTOCHESS_H
#define UI_MOTOCHESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MotoChess
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MotoChess)
    {
        if (MotoChess->objectName().isEmpty())
            MotoChess->setObjectName(QString::fromUtf8("MotoChess"));
        MotoChess->resize(800, 600);
        MotoChess->setSizeIncrement(QSize(0, 0));
        centralwidget = new QWidget(MotoChess);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MotoChess->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MotoChess);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MotoChess->setMenuBar(menubar);
        statusbar = new QStatusBar(MotoChess);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MotoChess->setStatusBar(statusbar);

        retranslateUi(MotoChess);

        QMetaObject::connectSlotsByName(MotoChess);
    } // setupUi

    void retranslateUi(QMainWindow *MotoChess)
    {
        MotoChess->setWindowTitle(QApplication::translate("MotoChess", "MotoChess", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MotoChess: public Ui_MotoChess {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOTOCHESS_H
