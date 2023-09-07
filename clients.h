#ifndef CLIENTS_H
#define CLIENTS_H

#include <QObject>
#include <QtNetwork>
#include <QVector>

class Clients : public QObject
{
    Q_OBJECT
public:
    explicit Clients(QObject *parent = nullptr);
    QString usernanme;
    QTcpSocket* socket;



signals:

};

#endif // CLIENTS_H
