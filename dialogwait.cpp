#include "dialogwait.h"
#include "ui_dialogwait.h"

DialogWait::DialogWait(QWidget *parent) :
    QDialog(parent), listenSocket(nullptr),
    ui(new Ui::DialogWait)
{
    ui->setupUi(this);
    setWindowTitle("等待中");
}

void DialogWait::set_enable()
{
    ui->label->setText("已连接");
}

void DialogWait::set_socket(QTcpServer *socket)
{
    listenSocket = socket;
}

DialogWait::~DialogWait()
{
    delete ui;
}

void DialogWait::on_btnCancel_clicked()
{
    listenSocket->close();
    delete listenSocket;
    close();
}
