#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QLCDNumber>
#include "chessboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "dialoginit.h"
#include "dialogwait.h"
#include "dialogconnect.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <iostream>
#include <QDataStream>
#include <QMessageBox>
#include <QThread>
#include <QLabel>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), is_red(true), listenSocket(nullptr), readWriteSocket(nullptr), _time(INTERVAL)
{
    ui->setupUi(this);

    window = new QWidget(this);
    setCentralWidget(window);
    btn_save = new QPushButton(window);
    btn_give_in = new QPushButton(window);
    btn_save->setText("保存");
    btn_give_in->setText("认输");
    btn_give_in->setEnabled(false);
    btn_save->setEnabled(false);
    connect(btn_give_in, SIGNAL(clicked(bool)), this, SLOT(give_in()));
    connect(btn_save, SIGNAL(clicked(bool)), this, SLOT(save_chess()));
    btn_save->setFixedWidth(CELL_SIZE * 3);
    btn_give_in->setFixedWidth(CELL_SIZE * 3);
    lcd_time = new QLCDNumber(window);
    lcd_time->setFixedWidth(CELL_SIZE * 3);
    lcd_time->display(INTERVAL);

    QHBoxLayout *top_layout = new QHBoxLayout;
    top_layout->addWidget(btn_save);
    top_layout->addWidget(btn_give_in);
    top_layout->addWidget(lcd_time);
    top_layout->setContentsMargins(0, 0, 0, 0);
    top_layout->setSpacing(0);

    QVBoxLayout *layout = new QVBoxLayout(window);
    layout->addLayout(top_layout);
    chess_board = new ChessBoard(window);
    connect(chess_board, SIGNAL(game_start()), this, SLOT(game_start()));
    connect(chess_board, SIGNAL(game_end()), this, SLOT(game_end()));
    connect(chess_board, SIGNAL(my_turn()), this, SLOT(timer_start()));
    connect(chess_board, SIGNAL(your_turn()), this, SLOT(timer_stop()));
    layout->addWidget(chess_board);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setFixedWidth(CELL_SIZE * 9);

    timer = new QTimer(window);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(dec_time()));

    setWindowTitle("QChineseChess");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionInitialize_Server_triggered()
{
    DialogInit dlginit(this);
    if (dlginit.exec() == QDialog::Accepted)
    {
        int port = dlginit.get_port();
        listenSocket = new QTcpServer;
        listenSocket->listen(QHostAddress::Any, port);
        connect(listenSocket, SIGNAL(newConnection()), this, SLOT(accept_connection()));
        dlgwait = new DialogWait(this);
        dlgwait->set_socket(listenSocket);
        dlgwait->exec();
    }
}

void MainWindow::accept_connection()
{
    ui->statusBar->addPermanentWidget(new QLabel("已连接"));
    dlgwait->close();
    delete dlgwait;
    readWriteSocket = listenSocket->nextPendingConnection();
    chess_board->setReadWriteSocket(readWriteSocket, true);
}

void MainWindow::on_actionConnect_triggered()
{
    DialogConnect dlgconnect(this);
    if (dlgconnect.exec() == QDialog::Accepted)
    {
        readWriteSocket = new QTcpSocket;
        QString host = dlgconnect.get_ip();
        int port = dlgconnect.get_port();
        readWriteSocket->connectToHost(QHostAddress(host), port);
        chess_board->setReadWriteSocket(readWriteSocket, false);
    }
}

void MainWindow::on_actionNew_Game_triggered()
{
    if (readWriteSocket)
    {
        is_red = true;
        game_end();
        chess_board->new_game(is_red, Utili::MOVE_ME);
        char *msg = new char[10];
        msg[0] = 'n';
        msg[1] = '\0';
        readWriteSocket->write(msg);
        btn_save->setEnabled(true);
        btn_give_in->setEnabled(true);
    }
}

void MainWindow::on_actionLoad_from_file_triggered()
{
    if (!readWriteSocket)
        return;
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open file"));
    if (fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];
        qDebug() << path;
        QFile *file = new QFile(path);
        is_red = true;
        chess_board->set_this_color(Utili::R);
        if(file->open(QFile::ReadOnly))
        {
            game_end();
            chess_board->clear_all();
            QTextStream in(file);
            QString all = in.readAll();
            QTextStream in2(&all);
            Utili::chess_color cc;
            int c = 0;
            //bool h = false;
            QString line = in2.readLine();
            if (line.at(0) == 'r')
            {
                chess_board->set_move(Utili::MOVE_ME);
                cc = Utili::R;
                c = 0;
            }
            else
            {
                chess_board->set_move(Utili::MOVE_OPPONENT);
                cc = Utili::B;
                c = 0;
            }
            while (!in2.atEnd())
            {
                line = in2.readLine();
                qDebug() << line;
                if (line.at(0) == 'r')
                {
                    cc = Utili::R;
                    c = 0;
                }
                else if (line.at(0) == 'b')
                {
                    cc = Utili::B;
                    c = 0;
                }
                else if (line.at(0).isDigit())
                {
                    c++; // chess type
                    int v = line.at(0).toLatin1() - '0';
                    std::cout << "v: " << v << std::endl;
                    int p = 0;
                    int a, b;
                    for (int i = 0; i < v; i++)
                    {
                        while (line.at(p) != '<')
                            p++;
                        a = line.at(p + 1).toLatin1() - '0';
                        b = line.at(p + 3).toLatin1() - '0';
                        p++;
                        std::cout << "a: " << a << " b: " << b << std::endl;
                        Utili::chess_type new_piece = Utili::get_chess_type_from_digit(c);
                        int index = (9 - b) * COL_COUNT + a;
                        if (!is_red)
                            index = 89 - index; // invert
                        chess_board->set_chess_piece(new_piece, cc, index);
                    }
                }
            }
            if (readWriteSocket)
            {
                QByteArray info;
                info = all.toLatin1();
                readWriteSocket->write(info);
            }
            game_start();
            if (chess_board->get_move() == Utili::MOVE_ME)
                timer_start();
        }
    }
}

void MainWindow::give_in()
{
    int ret = QMessageBox::warning(this, "QChineseChess", "确定要认输吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        lose();
    }
}

void MainWindow::game_start()
{
    btn_save->setEnabled(true);
    btn_give_in->setEnabled(true);
    _time = INTERVAL;
    lcd_time->display(_time);
    //timer_start();
}

void MainWindow::game_end()
{
    btn_save->setEnabled(false);
    btn_give_in->setEnabled(false);
    timer_stop();
}

void MainWindow::timer_start()
{
    _time = INTERVAL;
    lcd_time->display(_time);
    timer->start();
}

void MainWindow::timer_stop()
{
    lcd_time->display(INTERVAL);
    _time = INTERVAL;
    timer->stop();
}

void MainWindow::dec_time()
{
    if (--_time > 0)
        lcd_time->display(_time);
    else
    {
        lcd_time->display(30);
        _time = INTERVAL;
        timer->stop();
        lose();
    }
}

void MainWindow::save_chess()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存残局", "", "Chess files (*.txt)");
    QFile *file = new QFile(filename);
    cout << "aetrwesdf" << std::endl;
    chess_board->save_chess(file);
}

void MainWindow::win()
{

}

void MainWindow::lose()
{
    char *msg = new char[10];
    msg[0] = 'W';
    msg[1] = '\0';
    readWriteSocket->write(msg);
    timer_stop();
    btn_save->setEnabled(false);
    btn_give_in->setEnabled(false);
    QMessageBox::information(this, "QChineseChess", "您输了！", QMessageBox::Ok);
    chess_board->clear_all();
}
