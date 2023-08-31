#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "server.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

private:
    server *serverInstance;


public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

signals:
    void addressFound(QString address);

private slots:
    void closeServer();
    void pbDisconnect_pushed();


public slots:
    void handleClientCountChange(int newCount);
    void rCV(QStringList clientList);
    void rUV(QStringList userList);
    void updateTextbox();
};
#endif // WIDGET_H
