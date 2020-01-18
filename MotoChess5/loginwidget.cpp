#include "loginwidget.h"
#include <QGroupBox>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
LoginWidget::LoginWidget(MainWindow* parentApp) : QWidget(parentApp), parent(parentApp)
{
    this->setGeometry(parentApp->geometry());

    QGroupBox * loginGroup = new QGroupBox(this);
    loginGroup->setGeometry(this->width()/4, this->height()/4, this->width()/2, this->height()/2);

    QLayout* loginLayout = new QVBoxLayout(loginGroup);

    QLabel* loginInfo = new QLabel(loginGroup);
    loginInfo->setText("Enter your desired username: ");

    loginLine = new QLineEdit(loginGroup);

    QPushButton* loginButton = new QPushButton(loginGroup);
    loginButton->setText("Choose");

    loginLayout->addWidget(loginInfo);
    loginLayout->addWidget(loginLine);
    loginLayout->addWidget(loginButton);

    connect(loginButton, SIGNAL(clicked()), this, SLOT(attemptLogin()));
}

void LoginWidget::attemptLogin()
{
    if( parent->getConnectionStatus() == 0 )
        parent->ConnectToServer();

    if( 0>= write(parent->getSocket(), loginLine->text().toStdString().c_str(), 256) )
    {
        parent->getConnectionStatus() = 0;
        return;
    }

    char message[256];

    if( 0>= read(parent->getSocket(), message, 256) )
    {
        parent->getConnectionStatus() = 0;
        return;
    }

    if(!strcmp(message, "existsUser"))
    {
        QMessageBox* error = new QMessageBox();
        error->setText("User already connected.");
        error->setInformativeText("Select another user please");
        error->setStandardButtons(QMessageBox::Ok);
        error->setDefaultButton(QMessageBox::Ok);
        error->exec();
        return;
    }

    parent->getUsername() = loginLine->text();
    emit(connected());
}
