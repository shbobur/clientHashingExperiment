#include "client.h"

#include <QTime>

Client::Client(QString id, QObject *parent) :
    QObject(parent), name(id)
{
    socket = new QTcpSocket();
    connect(socket, SIGNAL(connected()),     this, SLOT(disconnected()));
    connect(socket, SIGNAL(disconnected()),  this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()),     this, SLOT(receiveData()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));

    port = 12345;
    IP = "127.0.0.1";

    timer = new QElapsedTimer();
}

Client::~Client()
{
    socket->disconnectFromHost();
    socket->close();
    socket->deleteLater();
}

void Client::connected()
{
    QString log = QString("Connected to %1\n").arg(url);
    emit workLog(log);
}

void Client::disconnected()
{
    //ui->consoleTextBrowser->insertPlainText(QString("Disconnected from the server.\n"));
}

void Client::receiveData()
{
    quint32 time = timer->elapsed();

    socket->waitForReadyRead(3000);
    QString log = QString("<font color=\"green\">Server</font>: %1").arg(QString(socket->readAll()));

    log += QString(",auth_time:|%1|secs<br>").arg(QString::number(time / 1000.0));

    emit workLog(log);

    socket->close();
}

void Client::sendData()
{
    socket->abort();
    socket->connectToHost(IP, port);

    socket->waitForConnected();
    QString requestID = QTime::currentTime().toString();
    socket->write(QString(name + "(" + requestID + ")").toLatin1());
    socket->waitForBytesWritten();

    timer->restart();

    QString log = QString("<font color=\"blue\">%1:(%2)</font>: Hello, Server<br>").arg(name, requestID);
    //emit workLog(log);
}

void Client::handleError(QAbstractSocket::SocketError err)
{
    QString errorStr = "<font color=\"red\">Socket error: </font>";
    switch(err) {
    case QAbstractSocket::ConnectionRefusedError: errorStr += "The connection was refused by the peer (or timed out).<br>"; break;
    //case QAbstractSocket::RemoteHostClosedError: errorStr += "The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.<br>"; break;
    case QAbstractSocket::HostNotFoundError: errorStr += "The host address was not found.<br>"; break;
    case QAbstractSocket::SocketAccessError: errorStr += "The socket operation failed because the application lacked the required privileges.<br>"; break;
    case QAbstractSocket::SocketResourceError: errorStr += "The local system ran out of resources (e.g., too many sockets).<br>"; break;
    case QAbstractSocket::SocketTimeoutError: errorStr += "The socket operation timed out."; break;
    case QAbstractSocket::DatagramTooLargeError: errorStr += "The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).<br>"; break;
    case QAbstractSocket::NetworkError: errorStr += "An error occurred with the network (e.g., the network cable was accidentally plugged out).<br>"; break;
    case QAbstractSocket::AddressInUseError: errorStr += "The address specified to QAbstractSocket::bind() is already in use and was set to be exclusive.<br>"; break;
    case QAbstractSocket::SocketAddressNotAvailableError: errorStr += "The address specified to QAbstractSocket::bind() does not belong to the host.<br>"; break;
    case QAbstractSocket::UnsupportedSocketOperationError: errorStr += "The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).<br>"; break;
    case QAbstractSocket::ProxyAuthenticationRequiredError: errorStr += "The socket is using a proxy, and the proxy requires authentication.<br>"; break;
    case QAbstractSocket::SslHandshakeFailedError: errorStr += "The SSL/TLS handshake failed, so the connection was closed (only used in QSslSocket)<br>"; break;
    case QAbstractSocket::UnfinishedSocketOperationError: errorStr += "Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).<br>"; break;
    case QAbstractSocket::ProxyConnectionRefusedError: errorStr += "Could not contact the proxy server because the connection to that server was denied<br>"; break;
    case QAbstractSocket::ProxyConnectionClosedError: errorStr += "The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established)<br>"; break;
    case QAbstractSocket::ProxyConnectionTimeoutError: errorStr += "The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.<br>"; break;
    case QAbstractSocket::ProxyNotFoundError: errorStr += "The proxy address set with setProxy() (or the application proxy) was not found.<br>"; break;
    case QAbstractSocket::ProxyProtocolError: errorStr += "The connection negotiation with the proxy server failed, because the response from the proxy server could not be understood.<br>"; break;
    case QAbstractSocket::OperationError: errorStr += "An operation was attempted while the socket was in a state that did not permit it.<br>"; break;
    case QAbstractSocket::SslInternalError: errorStr += "The SSL library being used reported an internal error. This is probably the result of a bad installation or misconfiguration of the library.<br>"; break;
    case QAbstractSocket::SslInvalidUserDataError: errorStr += "Invalid data (certificate, key, cypher, etc.) was provided and its use resulted in an error in the SSL library.<br>"; break;
    case QAbstractSocket::TemporaryError: errorStr += "A temporary error occurred (e.g., operation would block and socket is non-blocking).<br>"; break;
    default: errorStr = "";
    }

    emit socketError(errorStr);
}

QString Client::getIP() const
{
    return IP;
}

void Client::setIP(const QString &value)
{
    IP = value;
}

quint16 Client::getPort() const
{
    return port;
}

void Client::setPort(const quint16 &value)
{
    port = value;
}

void Client::sendRequest()
{
    sendData();
}

QString Client::getUrl() const
{
    return url;
}

void Client::setUrl(const QString &value)
{
    url = value;
}

QString Client::getName() const
{
    return name;
}

void Client::setName(const QString &value)
{
    name = value;
}
