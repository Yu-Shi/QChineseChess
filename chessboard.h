#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <vector>
#include "chesscell.h"
#include "utili.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QTextStream>
#include <QFile>
using namespace std;

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);
    void new_game(bool is_red, Utili::current_move m);

    void setReadWriteSocket(QTcpSocket *value, bool is_red);

    void set_chess_piece(Utili::chess_type type, Utili::chess_color color, int p);

    void set_move(Utili::current_move _move);
    Utili::current_move get_move() const;

    void receive_game(QByteArray str);

    Utili::chess_color get_this_color() const;

    void set_this_color(const Utili::chess_color &value);

    void clear_all();
    void save_chess(QFile *file);

signals:
    void game_start();
    void game_end();
    void my_turn();
    void your_turn();
    void win();

public slots:
    void receiveClick(ChessCell *c);
    void opponent_move();

private:
    vector<ChessCell *> cells;
    ChessCell *last_selected;
    Utili::chess_color this_color;
    bool _red;
    Utili::current_move _move;
    QTcpSocket  *readWriteSocket;
};

#endif // CHESSBOARD_H
