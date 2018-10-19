#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLCDNumber>
#include "chessboard.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include "dialogwait.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionInitialize_Server_triggered();
    void accept_connection();
    void on_actionConnect_triggered();
    void on_actionNew_Game_triggered();
    void on_actionLoad_from_file_triggered();
    void give_in();
    void game_start();
    void game_end();
    void timer_start();
    void timer_stop();
    void dec_time();
    void save_chess();

private:
    Ui::MainWindow *ui;
    QPushButton *btn_give_in;
    QPushButton *btn_save;
    ChessBoard *chess_board;
    QLCDNumber *lcd_time;
    QWidget *window;
    QTcpServer  *listenSocket;
    QTcpSocket  *readWriteSocket;
    DialogWait *dlgwait;
    bool is_red;
    QTimer *timer;
    int _time;

    void win();
    void lose();
};

#endif // MAINWINDOW_H
