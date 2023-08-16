#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>


class server : public QObject
{
    Q_OBJECT
private:
    QTcpServer  *tcpServer = nullptr;  //Server
    QTcpSocket  *socket = nullptr;  //Addres
    QVector< QVector<QString> > *commands();

    void createCommands();



public:
    explicit server(QObject *parent = nullptr);



signals:

public slots:
    void newClientConnection();
    void readyRead();
};

#endif // SERVER_H
