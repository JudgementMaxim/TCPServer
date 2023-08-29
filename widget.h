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

public slots:
    void handleClientCountChange(int newCount);
};
#endif // WIDGET_H
