#include "widget.h"
#include "ui_widget.h"

#include <QScrollBar>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    sendRequestTimer = new QTimer();
    updateLCDTimer = new QTimer();

    iterator = 0;
    allClientsNumber = 1024;

    createClients();
    setParameters();

    connect(ui->ipComboBox,             SIGNAL(currentTextChanged(QString)),    this,   SLOT(setParameters())   );
    connect(ui->portLineEdit,           SIGNAL(textChanged(QString)),           this,   SLOT(setParameters())   );
    connect(ui->numOfRequestsLineEdit,  SIGNAL(textChanged(QString)),           this,   SLOT(setParameters())   );
    connect(ui->intervalLineEdit,       SIGNAL(textChanged(QString)),           this,   SLOT(setParameters())   );
    connect(ui->startPushButton,        SIGNAL(clicked()),                      this,   SLOT(start())           );
    connect(ui->stopPushButton,         SIGNAL(clicked()),                      this,   SLOT(stop())            );
    connect(sendRequestTimer,           SIGNAL(timeout()),                      this,   SLOT(sendRequests())    );
    connect(updateLCDTimer,             SIGNAL(timeout()),                      this,   SLOT(updateLCDData())   );

}

Widget::~Widget()
{
    for (auto cl : allClients) {;
        cl->disconnect();
        cl->deleteLater();
    }
    allClients.clear();

    delete ui;
}

void Widget::writeToConsole(QString &logData)
{
    ui->consoleTextBrowser->insertHtml(logData);
    QScrollBar *sb = ui->consoleTextBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void Widget::setParameters()
{
    N = ui->numOfRequestsLineEdit->text().toInt();
    for (int i = 0; i < allClientsNumber; i++) {
        allClients[i]->setIP(ui->ipComboBox->currentText());
        allClients[i]->setPort(ui->portLineEdit->text().toInt());
    }
}

void Widget::sendRequests()
{
    for (int i = 0; i < N; i++, iterator++)
        allClients[iterator % allClientsNumber]->sendRequest();
}

void Widget::start()
{
    ui->parametersWidget->setDisabled(true);
    sendRequestTimer->start(ui->intervalLineEdit->text().toInt() * 1000);
    updateLCDTimer->start(200);

    updateLCDData();
    sendRequests();
}

void Widget::stop()
{
    ui->parametersWidget->setEnabled(true);
    sendRequestTimer->stop();
    updateLCDTimer->stop();
}

void Widget::updateLCDData()
{
    ui->timerLcdNumber->display(sendRequestTimer->remainingTime() / 1000.0);
}

void Widget::createClients()
{
    for (int i = 0; i < allClientsNumber; i++) {
        client = new Client(QString("client" + QString::number(i)));
        connect(client, SIGNAL(workLog(QString&)), this, SLOT(writeToConsole(QString&)));
        connect(client, SIGNAL(socketError(QString&)), this, SLOT(handleSocketError(QString&)));
        allClients.push_back(client);
    }
}

void Widget::on_cleareConsolePushButton_clicked()
{
    ui->consoleTextBrowser->clear();
}

void Widget::handleSocketError(QString &error)
{
    ui->consoleTextBrowser->insertHtml(error);
}
