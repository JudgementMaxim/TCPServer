#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QVector> // Include QVector

class server : public QObject
{
    Q_OBJECT
private:
    QTcpServer *tcpServer = nullptr; // Server
    QVector<QTcpSocket *> sockets;   // Use QVector and store pointers

    void connectToClient(QTcpSocket *socket,QString ip);

public:
    explicit server(QObject *parent = nullptr);

signals:

public slots:
    void newClientConnection();
    void readyRead();
    void writeToClient();
};

#endif // SERVER_H
