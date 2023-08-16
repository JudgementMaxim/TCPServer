#include "server.h"


server::server(QObject *parent) : QObject(parent)
{

    createCommands();

    tcpServer = new QTcpServer;    //Create Server
    tcpServer->listen(QHostAddress::Any,414); //Server listens to Port 414local

    connect(tcpServer,SIGNAL(newConnection()),this, SLOT(newClientConnection())); //When Client connects to server

}

void server::newClientConnection()
{
    QTcpSocket *newSocket = tcpServer->nextPendingConnection();
    socket = newSocket;

    socket->write("Connected\r\n");
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

void server::readyRead()
{
    while(socket->canReadLine()){
        QString line = socket->readLine().trimmed(); // Trim leading/trailing whitespace
        line.toUtf8();

        if(line == "#DATE"){
            socket->write(&QDate::currentDate().toString().toUtf8());
        }
        else if(line == "#DATE"){

        }
        else if(line == "#DATE"){
        }
        else if(line == "#DATE"){

        }
        else if(line == "#DATE"){

        }
        else if(line == "#DATE"){

        }else{

        }

        qDebug() << "Received: " << line;
    }
}




