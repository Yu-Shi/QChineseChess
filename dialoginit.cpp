#include "dialoginit.h"
#include "ui_dialoginit.h"

DialogInit::DialogInit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInit)
{
    ui->setupUi(this);
    setWindowTitle("建立主机");
}

int DialogInit::get_port()
{
    return ui->lineEdit->text().toInt();
}

DialogInit::~DialogInit()
{
    delete ui;
}
