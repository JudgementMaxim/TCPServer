#include "server.h"

server::server(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer;    //Create Server
    tcpServer->listen(QHostAddress::Any,414); //Server listens to Port 414 GangGang

    connect(tcpServer,SIGNAL(newConnection()),this, SLOT(newClientConnection())); //When Client connects to server
}

void server::newClientConnection()
{
    QTcpSocket *newSocket = tcpServer->nextPendingConnection();
    socket = newSocket;

    socket->write("Moin Mattrose \r\n");
}
