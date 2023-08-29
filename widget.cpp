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

