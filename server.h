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

    void messageToClient(QTcpSocket socket,QString message);

    void sendMessageToClient(QString message, QString clientAddress);

    QString ClientList();

    QStringList splitMessage(QString line);

    void disconnectClient(QTcpSocket *socket);

public:
    explicit server(QObject *parent = nullptr);

    int ClientCount = 0;

signals:
    void connectionUpdate();
public slots:
    void newClientConnection();
    void readyRead();

};

#endif // SERVER_H
