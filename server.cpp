#include "server.h"

server::server(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer;
    if (!tcpServer->listen(QHostAddress::Any, 5000)) {
        qDebug() << "Server could not start. Error: " << tcpServer->errorString();
        return;
    }
    connect(tcpServer, &QTcpServer::newConnection, this, &server::newClientConnection);
}

void server::newClientConnection()
{
    QTcpSocket *newSocket = tcpServer->nextPendingConnection();
    sockets.append(newSocket);
    increaseClientCount();
    clientList.append(newSocket->localAddress().toString());
    emit atCL(clientList);
    emit clientCountChanged(ClientCount);
    sendToClient(newSocket, "Connected\r\n");
    connect(newSocket, &QTcpSocket::readyRead, this, &server::readyRead);
}

void server::sendMessageToClient(QString message, QString clientAddress)
{
    for (QTcpSocket *clientSocket : sockets) {
        QString hostAddress = clientSocket->localAddress().toString();
        if (hostAddress == clientAddress) {
            sendToClient(clientSocket, message.toUtf8());
        }
    }
}

QString server::getClientList()
{
    QString clientListStr;
    for (QTcpSocket *clientSocket : sockets) {
        QString hostAddress = clientSocket->localAddress().toString();
        clientListStr.append(hostAddress + "\r\n");
    }
    return clientListStr;
}

void server::disconnectClient(QTcpSocket *socket)
{
    QString clientAddress = socket->localAddress().toString();
    int clientPlace = findClientPlace(clientAddress);
    int clientPlaceQList = findClientPlaceList(clientAddress);
    if (clientPlace != -1) {
        sockets.remove(clientPlace);
        clientList.removeAt(clientPlaceQList);
        decreaseClientCount();
        emit clientCountChanged(ClientCount);
        emit atCL(clientList);
        sendToClient(socket, "Disconnected");
        socket->disconnect();
    }
}

void server::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    qDebug() << "Reading available";
    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();
        if (line == "#DATE") {
            sendToClient(socket, QDate::currentDate().toString().toUtf8());
        } else if (line == "#CLIENTS") {
            QString clientListStr = getClientList();
            sendToClient(socket, clientListStr.toUtf8());
        } else if (line.startsWith("#MESSAGE ")) {
            QStringList parts = splitMessage(line);
            if (parts.size() == 3) {
                QString clientAddress = parts[1];
                QString message = parts[2];
                sendMessageToClient(message, clientAddress);
            }
        } else if (line == "#DISCONNECT") {
            sendToClient(socket, "Please close the program now \r\n");
            disconnectClient(socket);
        }
    }
}

void server::increaseClientCount()
{
    ClientCount++;
}

void server::decreaseClientCount()
{
    ClientCount--;
}

int server::findClientPlace(const QString &clientAddress)
{
    for (int i = 0; i < sockets.size(); i++) {
        QString hostAddress = sockets[i]->localAddress().toString();
        if (hostAddress == clientAddress) {
            return i;
        }
    }
    return -1;
}

int server::findClientPlaceList(const QString &clientAddress)
{
    for (int i = 0; i < clientList.size(); i++) {
        QString hostAddress = clientList[i];
        if (hostAddress == clientAddress) {
            return i;
        }
    }
    return -1;
}

void server::sendToClient(QTcpSocket *socket, const QByteArray &data)
{
    socket->write(data);
}

QStringList server::splitMessage(const QString &line)
{
    return line.split(" ");
}
