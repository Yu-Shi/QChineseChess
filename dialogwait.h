#ifndef DIALOGWAIT_H
#define DIALOGWAIT_H

#include <QDialog>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

namespace Ui {
class DialogWait;
}

class DialogWait : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWait(QWidget *parent = 0);
    void set_enable();
    void set_socket(QTcpServer *socket);
    ~DialogWait();

private slots:
    void on_btnCancel_clicked();

private:
    Ui::DialogWait *ui;
    QTcpServer  *listenSocket;
};

#endif // DIALOGWAIT_H
