#include "server.h"

server::server(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer;

    if (!tcpServer->listen(QHostAddress::Any, 414)) {
        qDebug() << "Server could not start. Error: " << tcpServer->errorString();
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newClientConnection()));
}


void server::newClientConnection()
{
    QTcpSocket *newSocket = tcpServer->nextPendingConnection();

    sockets.append(newSocket);  // Append the pointer to the socket
    newSocket->write("Connected\r\n");
    connect(newSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void server::readyRead()
{
    for (QTcpSocket *socket : sockets) {
        while (socket->canReadLine()) { // Use -> to access members of a pointer
            QString line = socket->readLine().trimmed();

            if (line == "#DATE") {
                socket->write(QDate::currentDate().toString().toUtf8());
            }
            else if(line == "#CLIENTS"){
                qDebug() << socket->localAddress();
                socket->write(socket->localAddress().toString().toUtf8());
            }
        }
    }
}







