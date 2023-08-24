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
    server server;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
public slots:
    void changeClientNumber(int number);
};
#endif // WIDGET_H
