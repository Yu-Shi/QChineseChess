#include "dialogconnect.h"
#include "ui_dialogconnect.h"
#include <QMessageBox>

DialogConnect::DialogConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConnect)
{
    ui->setupUi(this);
    setWindowTitle("连接到主机");
}

QString DialogConnect::get_ip()
{
    return ui->edtIP->text();
}

int DialogConnect::get_port()
{
    return ui->edtPort->text().toInt();
}

bool DialogConnect::isIPAddressValid(const char *pszIPAddr)
{

    if (!pszIPAddr) return false;
    char IP1[100],cIP[4];
    int len = strlen(pszIPAddr);
    int i = 0,j=len-1;
    int k, m = 0,n=0,num=0;
    while (pszIPAddr[i++] == ' ');
    while (pszIPAddr[j--] == ' ');

    for (k = i-1; k <= j+1; k++)
    {
        IP1[m++] = *(pszIPAddr + k);
    }
    IP1[m] = '\0';

    char *p = IP1;

    while (*p!= '\0')
    {
        if (*p == ' ' || *p<'0' || *p>'9') return false;
        cIP[n++] = *p;

        int sum = 0;
        while (*p != '.'&&*p != '\0')
        {
          if (*p == ' ' || *p<'0' || *p>'9') return false;
          sum = sum * 10 + *p-48;
          p++;
        }
        if (*p == '.') {
            if ((*(p - 1) >= '0'&&*(p - 1) <= '9') && (*(p + 1) >= '0'&&*(p + 1) <= '9'))
                num++;
            else
                return false;
        };
        if ((sum > 255) || (sum > 0 && cIP[0] =='0')||num>3) return false;

        if (*p != '\0') p++;
        n = 0;
    }
    if (num != 3) return false;
    return true;
}

DialogConnect::~DialogConnect()
{
    delete ui;
}

void DialogConnect::on_buttonBox_accepted()
{
    QString ipstr = get_ip();
    if (isIPAddressValid(ipstr.toLatin1().data()))
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this, "QChineseChess", "IP地址不合法！", QMessageBox::Ok);
    }
}
