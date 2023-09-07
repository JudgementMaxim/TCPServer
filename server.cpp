#include "server.h"

server::server(QObject *parent) : QObject(parent)
{
    // Initialize the TCP server
    tcpServer = new QTcpServer;
    if (!tcpServer->listen(QHostAddress::Any, 5000)) {
        qDebug() << "Server could not start. Error: " << tcpServer->errorString();
        return;
    }

    // Initialize the SQLite database
    sqliteDatabase = QSqlDatabase::addDatabase("QSQLITE");
    sqliteDatabase.setDatabaseName("TCPServer.sqlite");

    // Connect to the database
    connectDatabase();

    // Connect signals and slots
    connect(tcpServer, &QTcpServer::newConnection, this, &server::newClientConnection);
}

void server::newClientConnection()
{
    // Handle a new client connection
    QTcpSocket *newSocket = tcpServer->nextPendingConnection();
    sockets.append(newSocket);
    increaseClientCount();
    clientList.append(newSocket->localAddress().toString());
    emit atCL(clientList);
    emit clientCountChanged(ClientCount);
    sendToClient(newSocket, "Connected\r\n");
    connect(newSocket, &QTcpSocket::readyRead, this, &server::readyRead);
}

void server::readyRead()
{
    // Handle data received from clients
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();
        processClientCommand(socket, line);
        emit newCommandSend();
    }
}

void server::processClientCommand(QTcpSocket *socket, const QString &line)
{
    // Process commands from clients
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

void server::sendMessageToClient(const QString &message, const QString &clientAddress)
{
    // Send a message to a specific client
    for (QTcpSocket *clientSocket : sockets) {
        if (clientSocket->localAddress().toString() == clientAddress) {
            sendToClient(clientSocket, message.toUtf8());
            return; // No need to continue searching
        }
    }
}

QString server::getClientList()
{
    // Generate a list of connected clients
    QString clientListStr;
    for (QTcpSocket *clientSocket : sockets) {
        clientListStr.append(clientSocket->localAddress().toString() + "\r\n");
    }
    return clientListStr;
}

void server::disconnectClient(QTcpSocket *socket)
{
    // Disconnect and remove a client
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

void server::increaseClientCount()
{
    // Increase the client count
    ClientCount++;
}

void server::decreaseClientCount()
{
    // Decrease the client count
    ClientCount--;
}

void server::connectDatabase()
{
    // Connect to the SQLite database
    if (!sqliteDatabase.open()) {
        qDebug() << "Database connection failed!";
        return; // Handle the error as needed
    } else {
        qDebug() << "Database connection succeeded";
    }
}

void server::disconnectDatabase()
{
    // Disconnect from the database
    sqliteDatabase.close();
}

int server::findClientPlace(const QString &clientAddress)
{
    // Find the index of a client in the sockets list
    for (int i = 0; i < sockets.size(); i++) {
        if (sockets[i]->localAddress().toString() == clientAddress) {
            return i;
        }
    }
    return -1;
}

int server::findClientPlaceList(const QString &clientAddress)
{
    // Find the index of a client in the clientList
    return clientList.indexOf(clientAddress);
}

void server::sendToClient(QTcpSocket *socket, const QByteArray &data)
{
    // Send data to a client
    socket->write(data);
}

QStringList server::splitMessage(const QString &line)
{
    // Split a message into parts
    return line.split(" ");
}

// Add this member variable to your server class


void server::userLogin()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    while (socket->canReadLine()) {
        socket->write("Please provide your username and password (Username Password)\r\n");
        QString line = socket->readLine().trimmed();
        QStringList parts = line.split(" ");
        if (parts.size() == 2) {
            QString username = parts[0];
            QString password = parts[1];
            QSqlQuery query;
            query.prepare("SELECT password FROM your_table WHERE username = :username");
            query.bindValue(":username", username);
            if (query.exec()) {
                if (query.next()) {
                    QString passwordServer = query.value(0).toString();
                    if (password == passwordServer) {
                        socket->write("Login successful\r\n");

                        // Store the logged-in client's socket
                        loggedInClients[username] = socket;

                        // You can also emit a signal or perform any other actions for a successful login
                    } else {
                        socket->write("Incorrect password\r\n");
                    }
                } else {
                    socket->write("User does not exist\r\n");
                }
            } else {
                qDebug() << "Query failed:" << query.lastError();
            }
        }
    }
}

// Function to send a message to a specific logged-in client
void server::sendMessageToLoggedInClient(QString username, QString message)
{
    if (loggedInClients.contains(username)) {
        QTcpSocket *clientSocket = loggedInClients[username];
        sendToClient(clientSocket, message.toUtf8());
    } else {
        qDebug() << "Client " << username << " is not logged in.";
        // Handle the case where the client is not logged in
    }
}

// Function to send a message to all logged-in clients
void server::broadcastMessageToLoggedInClients(QString message)
{
    for (const QString &username : loggedInClients.keys()) {
        QTcpSocket *clientSocket = loggedInClients[username];
        sendToClient(clientSocket, message.toUtf8());
    }
}

// Function to log out a client

