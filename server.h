#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QVector>

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

public:
    QString  allCommands;
    explicit server(QObject *parent = nullptr);

signals:
    void clientCountChanged(int newCount);
    void atCL(QStringList clientList);
    void newCommandSend();

public slots:
    void newClientConnection();
    void readyRead();
    void sendMessageToClient(QString message, QString clientAddress);
    QString getClientList();
    void disconnectClient(QTcpSocket *socket);
    void getSocketforDisconnect(QString address);
};

#endif // SERVER_H
