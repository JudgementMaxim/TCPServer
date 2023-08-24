#include "server.h"




server::server(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer;

    if (!tcpServer->listen(QHostAddress::Any, 5000)) {
        qDebug() << "Server could not start. Error: " << tcpServer->errorString();
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newClientConnection()));
    // Assuming you have instances of server and Widget classes named 'this' and 'widget'
    connect(this, &server::connectionUpdate, &widget, &Widget::changeClientNumber);


}

void server::newClientConnection()
{
    QTcpSocket *newSocket = tcpServer->nextPendingConnection();
    sockets.append(newSocket);
    ClientCount +=1;
    emit connectionUpdate();

    newSocket->write("Connected\r\n");
    connect(newSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}


void server::sendMessageToClient(QString message, QString clientAddress)
{
    for (QTcpSocket *clientSocket : sockets) {
        QString hostAddress = clientSocket->localAddress().toString();
        if (hostAddress == clientAddress){
            clientSocket->write(message.toUtf8());
        }

    }
}

QString server::ClientList()
{
    QString clientList;
    for (QTcpSocket *clientSocket : sockets) {
        QString hostAddress = clientSocket->localAddress().toString();
        //QString address = QString::number(hostAddress);
        clientList.append(hostAddress + "\r\n");
    }

    return clientList;
}

void server::disconnectClient(QTcpSocket *socket)
{
    int clientPlace = 0;
    for (QTcpSocket *clientSocket : sockets) {
        QString hostAddress = clientSocket->localAddress().toString();
        QString clientAddress = socket->localAddress().toString();
        if (hostAddress == clientAddress){
            sockets.remove(clientPlace);
            socket->write("Disconnected");
            socket->disconnect();
            ClientCount -=1;
            qDebug() << clientPlace;
        }
        clientPlace +=1;
        qDebug() << clientPlace;
    }
}

void server::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender()); // Get the socket that emitted the signal

    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();

        if (line == "#DATE") {
            socket->write(QDate::currentDate().toString().toUtf8());
        }
        else if (line == "#CLIENTS") {
            QString clientList = ClientList();
            socket->write(clientList.toUtf8());
        }
        else if (line.startsWith("#MESSAGE ")) {
            QStringList parts = line.split(" ");
            if (parts.size() == 3) {
                QString clientAddress = parts[1];
                QString message = parts[2];
                sendMessageToClient(message, clientAddress);
            }
        }
        else if(line == "#DISCONNECT"){
            disconnectClient(socket);
        }
    }
}


