#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>
#include <QTimer>

#include "client.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void writeToConsole(QString &logData);
    void setParameters();

    void sendRequests();
    void start();
    void stop();

    void updateLCDData();

    void on_cleareConsolePushButton_clicked();

    void handleSocketError(QString &error);

private:
    Ui::Widget *ui;

    Client *client;
    quint8 N;
    quint16 allClientsNumber;
    quint16 iterator;
    std::vector<Client*> allClients;

    QTimer *sendRequestTimer;
    QTimer *updateLCDTimer;

    void createClients();

};

#endif // WIDGET_H
