#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QTcpSocket>
#include <QElapsedTimer>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QString id, QObject *parent = 0);
    ~Client();

    QString getName() const;
    void setName(const QString &value);

    QString getUrl() const;
    void setUrl(const QString &value);

    quint16 getPort() const;
    void setPort(const quint16 &value);

    QString getIP() const;
    void setIP(const QString &value);

    void sendRequest();

signals:
    void workLog(QString &str);
    void socketError(QString &error);

private slots:
    void connected();
    void disconnected();
    void receiveData();
    void sendData();
    void handleError(QAbstractSocket::SocketError err);

private:
    QString name;
    QString url;
    quint16 port;
    QString IP;

    QTcpSocket *socket;
    QElapsedTimer *timer;
};

#endif // CLIENT_H
