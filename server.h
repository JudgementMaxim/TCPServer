#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QVector>
#include <QtSql>

#include "clients.h"

class server : public QObject
{
    Q_OBJECT
private:
    QTcpServer *tcpServer = nullptr;

    QVector<QTcpSocket *> sockets;
    QStringList clientList;
    bool loggedIn = false;
    int ClientCount = 0;

    void sendToClient(QTcpSocket *socket, const QByteArray &data);
    QStringList splitMessage(const QString &line);
    int findClientPlace(const QString &clientAddress);
    int findClientPlaceList(const QString &clientAddress);
    void increaseClientCount();
    void decreaseClientCount();

    //Database

    QSqlDatabase sqliteDatabase;
    void connectDatabase();
    void disconnectDatabase();


    void sendMessageToLoggedInClient(QString username, QString message);
    void broadcastMessageToLoggedInClients(QString message);
    void logoutClient(QString username);
    void processClientCommand(QTcpSocket *socket, const QString &line);

public:
    QMap<QString, QTcpSocket *> loggedInClients;
    QString  allCommands;
    explicit server(QObject *parent = nullptr);

signals:
    void clientCountChanged(int newCount);
    void atCL(QStringList clientList);
    void newCommandSend();

public slots:
    void newClientConnection();
    void readyRead();
    void sendMessageToClient(const QString &message, const QString &clientAddress);
    QString getClientList();
    void disconnectClient(QTcpSocket *socket);
    void getSocketforDisconnect(QString address);

    //Database
    void userLogin();
};


#endif // SERVER_H
