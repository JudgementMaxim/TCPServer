#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // Instantiate your server using dynamic memory allocation
    serverInstance = new server(this);


    // Connect the signal from the server to the slot in the Widget
    connect(serverInstance, &server::clientCountChanged, this, &Widget::handleClientCountChange);
    connect(serverInstance, &server::atCL,this, &Widget::rCV);


    connect(ui->pbExit,SIGNAL(clicked()),this,SLOT(closeServer()));
    connect(ui->pbDisconnect,SIGNAL(clicked()),this,SLOT(pbDisconnect_pushed()));

    connect(this,&Widget::addressFound, serverInstance, &server::getSocketforDisconnect);

}

// ...

void Widget::handleClientCountChange(int newCount)
{
    // Update the UI to reflect the new client count
    ui->lcdClientCount->display(newCount);
}



Widget::~Widget()
{
    delete serverInstance; // Release the dynamically allocated memory
    delete ui;
}

void Widget::closeServer()
{
    exit(0);
}

void Widget::pbDisconnect_pushed()
{
    QModelIndex index = ui->lwClients->currentIndex();
    QString address = index.data(Qt::DisplayRole).toString();
    qDebug() << address;
    emit addressFound(address);
}

void Widget::rCV(QStringList clientList)
{
    ui->lwClients->clear();
    ui->lwClients->addItems(clientList);
}

void Widget::rUV(QStringList userList)
{
    ui->lwUsers->clear();
    ui->lwUsers->addItems(userList);
}

