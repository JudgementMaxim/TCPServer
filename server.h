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

    void sendMessageToClient(QString message, QString clientAddress);

    QString ClientList();

    void disconnectClient(QTcpSocket *socket);

    bool loggedIn = false;

public:
    explicit server(QObject *parent = nullptr);

    int ClientCount = 0;

signals:
    void clientCountChanged(int newCount);

public slots:
    void newClientConnection();
    void readyRead();
};

#endif // SERVER_H
