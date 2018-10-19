#ifndef DIALOGCONNECT_H
#define DIALOGCONNECT_H

#include <QDialog>

namespace Ui {
class DialogConnect;
}

class DialogConnect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConnect(QWidget *parent = 0);
    QString get_ip();
    int get_port();

    bool isIPAddressValid(const char* pszIPAddr);
    ~DialogConnect();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogConnect *ui;
};

#endif // DIALOGCONNECT_H
