#include "server.h"

server::server(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer;

    if (!tcpServer->listen(QHostAddress::Any, 5000)) {
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
            QString clientList;
            for (QTcpSocket *clientSocket : sockets) {
                QString hostAddress = clientSocket->localAddress().toString();
                //QString address = QString::number(hostAddress);
                clientList.append(hostAddress + "\r\n");
            }
            socket->write(clientList.toUtf8());
            qDebug() << clientList;
        } else if (line.startsWith("#CONNECT ")) {
            QStringList parts = line.split(" ");
            if (parts.size() == 2) {
                QString clientPort = parts[1];
                connectToClient(socket, clientPort);
            }
        }else if(line == "DISCONNECT"){

        }

    }
}

void server::writeToClient()
{

}

void server::connectToClient(QTcpSocket *originSocket, QString ip)
{
    for (QTcpSocket *socket : sockets){
        if (socket->localAddress().toString() == ip){
            socket->write("Client with folowing address connected: " + originSocket->localAddress().toString().toUtf8() + "\r\n");
            originSocket->write("Connected to client with address " + originSocket->localAddress().toString().toUtf8() + "\r\n");
            connect(socket, SIGNAL(readyRead()), this, SLOT(writeToClient()));
        }else if(socket->localAddress().toString() == originSocket->localAddress().toString()){
            qDebug()<< "Own Socket";
        }else{
            originSocket->write("Client not foucnd\r\n");
        }
    }
}
