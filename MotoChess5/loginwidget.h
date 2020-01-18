#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H
#include "mainwindow.h"

#include <QWidget>

#include <QLineEdit>
class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    LoginWidget(MainWindow*);
signals:
    void connected();
private slots:
    void attemptLogin();
private:
    MainWindow* parent;
    QLineEdit* loginLine;
};

#endif // LOGINWIDGET_H
