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

    sockets.append(newSocket);
    newSocket->write("Connected\r\n");
    connect(newSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void server::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender()); // Get the socket that emitted the signal

    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();

        if (line == "#DATE") {
            socket->write(QDate::currentDate().toString().toUtf8());
        } else if (line == "#CLIENTS") {
            QByteArray clientList;
            for (QTcpSocket *clientSocket : sockets) {
                clientList.append(QString::number(clientSocket->localPort()) + "\r\n");
            }
            socket->write(clientList);
        } else if (line.startsWith("#CONNECT ")) {
            QStringList parts = line.split(" ");
            if (parts.size() == 2) {
                int clientPort = parts[1].toInt();
                connectToClient(socket, clientPort);
            }
        }
    }
}

void server::connectToClient(QTcpSocket *originSocket, int clientPort)
{
    for (QTcpSocket *socket : sockets) {
        if (socket->localPort() == clientPort) {
            // Send a message to the target client
            QString message = "Connected to client on port " + QString::number(originSocket->localPort()) + "\r\n";
            socket->write(message.toUtf8());
            return; // Exit the loop since we've found the target client
        }
    }

    // If target client not found, send a message back to the origin client
    originSocket->write("Client not found\r\n");
}
