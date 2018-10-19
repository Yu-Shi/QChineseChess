#include "chessboard.h"
#include "utili.h"
#include "chesscell.h"
#include <QGridLayout>
#include <iostream>
#include <QMessageBox>
#include <QtMultimedia/QSound>
#include <QMediaPlayer>

ChessBoard::ChessBoard(QWidget *parent) : QWidget(parent), last_selected(nullptr)
{
    QGridLayout *layout = new QGridLayout(this);
    for (int i = 0; i < CELL_SUM; i++)
    {
        ChessCell *cell = new ChessCell(this);
        cell->setId(i);
        cell->set_cell_type(Utili::POS_NORMAL);
        layout->addWidget(cell, i / COL_COUNT, i % COL_COUNT, 1, 1);
        cell->set_row_num(i / COL_COUNT);
        cell->set_col_num(i % COL_COUNT);
        connect(cell, SIGNAL(mousePressed(ChessCell*)), this, SLOT(receiveClick(ChessCell*)));
        cells.push_back(cell);
    }
    for (int i = 0; i < CELL_SUM; i++)
    {
        cells[i]->set_mates(cells);
    }
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setMargin(0);
    layout->setSpacing(0);

    for (int i = 1; i < COL_COUNT - 1; i++)
    {
        cells[COL_COUNT * 4 + i]->set_cell_type(Utili::POS_RIVER_TOP_EDGE);
    }
    for (int i = 1; i < COL_COUNT - 1; i++)
    {
        cells[COL_COUNT * 5 + i]->set_cell_type(Utili::POS_RIVER_BOTTOM_EDGE);
    }

    cells[1]->set_cell_type(Utili::POS_TOP);
    cells[2]->set_cell_type(Utili::POS_TOP);
    cells[4]->set_cell_type(Utili::POS_TOP);
    cells[6]->set_cell_type(Utili::POS_TOP);
    cells[7]->set_cell_type(Utili::POS_TOP);
    cells[COL_COUNT * 9 + 1]->set_cell_type(Utili::POS_BOTTOM);
    cells[COL_COUNT * 9 + 2]->set_cell_type(Utili::POS_BOTTOM);
    cells[COL_COUNT * 9 + 4]->set_cell_type(Utili::POS_BOTTOM);
    cells[COL_COUNT * 9 + 6]->set_cell_type(Utili::POS_BOTTOM);
    cells[COL_COUNT * 9 + 7]->set_cell_type(Utili::POS_BOTTOM);
    for (int i = 1; i < ROW_COUNT - 1; i++)
    {
        cells[i * COL_COUNT]->set_cell_type(Utili::POS_LEFT);
        cells[i * COL_COUNT + 8]->set_cell_type(Utili::POS_RIGHT);
    }
    cells[INDEX_TOP_LEFT]->set_cell_type(Utili::POS_TOP_LEFT);
    cells[INDEX_TOP_RIGHT]->set_cell_type(Utili::POS_TOP_RIGHT);
    cells[INDEX_BOTTOM_LEFT]->set_cell_type(Utili::POS_BOTTOM_LEFT);
    cells[INDEX_BOTTOM_RIGHT]->set_cell_type(Utili::POS_BOTTOM_RIGHT);
    cells[INDEX_TOP_LEFT + 3]->set_cell_type(Utili::POS_PALACE_TOP_LEFT_TOP);
    cells[INDEX_TOP_LEFT + 5]->set_cell_type(Utili::POS_PALACE_TOP_RIGHT_TOP);
    cells[COL_COUNT * 2 + 3]->set_cell_type(Utili::POS_PALACE_BOTTOM_LEFT);
    cells[COL_COUNT * 2 + 5]->set_cell_type(Utili::POS_PALACE_BOTTOM_RIGHT);
    cells[COL_COUNT + 4]->set_cell_type(Utili::POS_PALACE_CENTER);
    cells[COL_COUNT * 7 + 3]->set_cell_type(Utili::POS_PALACE_TOP_LEFT);
    cells[COL_COUNT * 7 + 5]->set_cell_type(Utili::POS_PALACE_TOP_RIGHT);
    cells[COL_COUNT * 8 + 4]->set_cell_type(Utili::POS_PALACE_CENTER);
    cells[COL_COUNT * 9 + 3]->set_cell_type(Utili::POS_PALACE_BOTTOM_LEFT_BOTTOM);
    cells[COL_COUNT * 9 + 5]->set_cell_type(Utili::POS_PALACE_BOTTOM_RIGHT_BOTTOM);
    cells[COL_COUNT * 4]->set_cell_type(Utili::POS_RIVER_TOP_EDGE_LEFT);
    cells[COL_COUNT * 4 + 8]->set_cell_type(Utili::POS_RIVER_TOP_EDGE_RIGHT);
    cells[COL_COUNT * 5]->set_cell_type(Utili::POS_RIVER_BOTTOM_EDGE_LEFT);
    cells[COL_COUNT * 5 + 8]->set_cell_type(Utili::POS_RIVER_BOTTOM_EDGE_RIGHT);
    setFixedSize(COL_COUNT * CELL_SIZE, ROW_COUNT * CELL_SIZE);
}

void ChessBoard::new_game(bool is_red, Utili::current_move m)
{
    clear_all();
    _red = is_red;
    _move = m;
    this_color = (is_red) ? Utili::R : Utili::B;
    if (is_red)
        emit my_turn();
    cells[81]->set_chess_piece(Utili::JU, this_color);
    cells[82]->set_chess_piece(Utili::MA, this_color);
    cells[83]->set_chess_piece(Utili::XIANG, this_color);
    cells[84]->set_chess_piece(Utili::SHI, this_color);
    cells[85]->set_chess_piece(Utili::JIANGSHUAI, this_color);
    cells[86]->set_chess_piece(Utili::SHI, this_color);
    cells[87]->set_chess_piece(Utili::XIANG, this_color);
    cells[88]->set_chess_piece(Utili::MA, this_color);
    cells[89]->set_chess_piece(Utili::JU, this_color);
    cells[70]->set_chess_piece(Utili::PAO, this_color);
    cells[64]->set_chess_piece(Utili::PAO, this_color);
    for (int i = 54; i <= 62; i += 2)
    {
        cells[i]->set_chess_piece(Utili::ZUBING, this_color);
    }

    Utili::chess_color op_color = Utili::get_opposite_color(this_color);
    cells[0]->set_chess_piece(Utili::JU, op_color);
    cells[1]->set_chess_piece(Utili::MA, op_color);
    cells[2]->set_chess_piece(Utili::XIANG, op_color);
    cells[3]->set_chess_piece(Utili::SHI, op_color);
    cells[4]->set_chess_piece(Utili::JIANGSHUAI, op_color);
    cells[5]->set_chess_piece(Utili::SHI, op_color);
    cells[6]->set_chess_piece(Utili::XIANG, op_color);
    cells[7]->set_chess_piece(Utili::MA, op_color);
    cells[8]->set_chess_piece(Utili::JU, op_color);
    cells[19]->set_chess_piece(Utili::PAO, op_color);
    cells[25]->set_chess_piece(Utili::PAO, op_color);
    for (int i = 27; i <= 35; i += 2)
    {
        cells[i]->set_chess_piece(Utili::ZUBING, op_color);
    }
}

void ChessBoard::receiveClick(ChessCell *c)
{
    for (auto cell : cells)
    {
        if (cell == c)
            cell->set_selected(true);
        else
            cell->set_selected(false);
    }
    if (_move == Utili::MOVE_OPPONENT)
    {
        last_selected = nullptr;
        return;
    }
    if (last_selected && last_selected->get_chess_color() == Utili::get_opposite_color(this_color))
    {
        last_selected = c;
        return;
    }
    ChessCell *pre = last_selected;
    if (pre && pre->get_chess_piece() != Utili::NO_PIECE)
    {
        if (pre == c)
        {
            last_selected = nullptr;
            return;
        }
        if (pre->get_chess_color() == c->get_chess_color())
        {
            last_selected = c;
            return;
        }
        for (auto i : pre->get_threatening_pos())
        {
            if (cells[i] == c) // move successfully here
            {
                bool f = false;
                if (c->get_chess_piece() == Utili::JIANGSHUAI)
                {
                    f = true;
                }
                cout << "hello43423" << std::endl;
                Utili::chess_type t = pre->get_chess_piece();
                c->set_chess_piece(pre->get_chess_piece(), pre->get_chess_color());
                pre->set_chess_piece(Utili::NO_PIECE, Utili::NO_COLOR);
                _move = Utili::MOVE_OPPONENT;
                char *msg = new char[10];
                msg[0] = Utili::get_char_chess_type(t);
                msg[1] = pre->getId();
                msg[2] = c->getId();
                emit your_turn();
                last_selected = nullptr;

                if (!f)
                    msg[3] = '0';
                else
                {
                    msg[3] = 'w';
                    emit game_end();
                    msg[4] = '\0';
                    readWriteSocket->write(msg);
                    QMessageBox::information(this, "QChineseChess", "您赢了！", QMessageBox::Ok);
                    clear_all();
                    return;
                }

                auto d = c;
                for (int i = 0; i < 89; i++)
                {
                    d = cells[i];
                    if (d->get_chess_color() == this_color)
                    {
                        for (auto j : d->get_threatening_pos(true))
                        {
                            if (cells[j]->get_chess_piece() == Utili::JIANGSHUAI && cells[j]->get_chess_color() == Utili::get_opposite_color(this_color))
                            {
                                //QMessageBox::information(this, "QChineseChess", "将军！", QMessageBox::Ok);
                                QMediaPlayer *player = new QMediaPlayer;
                                player->setMedia(QMediaContent(QUrl::fromLocalFile("D:\\projects\\QChineseChess_Server\\check.mp3")));
                                //QUrl::from
                                player->setVolume(100);
                                player->play();
                                msg[3] = 'j';
                                break;
                            }
                        }
                    }
                }
                msg[4] = '\0';
                readWriteSocket->write(msg);
                return;
            }
        }
        last_selected = c;
        return;
        /*c->set_chess_piece(pre->get_chess_piece(), pre->get_chess_color());
        pre->set_chess_piece(Utili::NO_PIECE, Utili::NO_COLOR);
        last_selected = nullptr;*/
    }
    else
    {
        last_selected = c;
    }
}

void ChessBoard::opponent_move()
{
    //char *msg = new char[10];
    //readWriteSocket->read(msg, 10);
    QByteArray bmsg = readWriteSocket->readAll();
    if (bmsg.at(0) == 'n')
    {
        new_game(false, Utili::MOVE_OPPONENT);
        emit game_start();
        return;
    }
    if (bmsg.at(0) == 'b' || bmsg.at(0) == 'r')
    {
        receive_game(bmsg);
        emit game_start();
        if (_move == Utili::MOVE_ME)
            emit my_turn();
        return;
    }
    if (bmsg.at(0) == 'W')
    {
        emit game_end();
        QMessageBox::information(this, "QChineseChess", "您赢了！", QMessageBox::Ok);
        clear_all();
        return;
    }
    Utili::chess_type t = Utili::get_chess_type_from_char(bmsg.at(0));
    int preid = bmsg.at(1);
    int cid = bmsg.at(2);
    if (preid != 90 && preid != 91)
    {
        cells[89 - preid]->set_chess_piece(Utili::NO_PIECE, Utili::NO_COLOR);
        cells[89 - cid]->set_chess_piece(t, Utili::get_opposite_color(this_color));
        _move = Utili::MOVE_ME;
        emit my_turn();
        if (bmsg.at(3) == 'j')
        {
            QMediaPlayer *player = new QMediaPlayer;
            player->setMedia(QMediaContent(QUrl::fromLocalFile("D:\\projects\\QChineseChess_Server\\check.mp3")));
            //QUrl::from
            player->setVolume(100);
            player->play();
        }
        else if (bmsg.at(3) == 'w')
        {
            emit game_end();
            QMessageBox::information(this, "QChineseChess", "您输了！", QMessageBox::Ok);
            clear_all();
            return;
        }
    }
    else
    {
        std::cout << "here!" << std::endl;
        cells[89 - cid]->set_chess_piece(t, (bmsg.at(3) == 0) ? Utili::B : Utili::R);
        if (preid == 90)
            _move = Utili::MOVE_OPPONENT;
        else if (preid == 91)
            _move = Utili::MOVE_ME;
    }
}

void ChessBoard::set_this_color(const Utili::chess_color &value)
{
    this_color = value;
}

void ChessBoard::clear_all()
{
    for (auto i : cells)
    {
        i->set_chess_piece(Utili::NO_PIECE, Utili::NO_COLOR);
    }
}

void ChessBoard::save_chess(QFile *file)
{
    if (file->open(QFile::WriteOnly))
    {
        cout << "Rfdsadfv" << endl;
        QTextStream out(file);
        QString lines[16];
        int r;
        if (_move == Utili::MOVE_ME)
        {
            lines[0] = (this_color == Utili::R) ? "red" : "black";
            lines[8] = (this_color == Utili::R) ? "black" : "red";
            r = (this_color == Utili::R) ? 0 : 8;
        }
        else
        {
            lines[8] = (this_color == Utili::R) ? "red" : "black";
            lines[0] = (this_color == Utili::R) ? "black" : "red";
            r = (this_color == Utili::R) ? 8 : 0;
        }
        vector<int> pieces[7];
        cout << 2 << endl;
        for (auto cell : cells)
        {
            if (cell->get_chess_color() == Utili::R)
            {
                switch(cell->get_chess_piece())
                {
                case Utili::JIANGSHUAI:
                    pieces[0].push_back(cell->getId());
                    break;
                case Utili::SHI:
                    pieces[1].push_back(cell->getId());
                    break;
                case Utili::XIANG:
                    pieces[2].push_back(cell->getId());
                    break;
                case Utili::MA:
                    pieces[3].push_back(cell->getId());
                    break;
                case Utili::JU:
                    pieces[4].push_back(cell->getId());
                    break;
                case Utili::PAO:
                    pieces[5].push_back(cell->getId());
                    break;
                case Utili::ZUBING:
                    pieces[6].push_back(cell->getId());
                    break;
                default:
                    break;
                }
            }
        }
        cout << 3 << endl;
        for (int i = 1; i <= 7; i++)
        {
            lines[i + r].append(QString::number(pieces[i - 1].size()));//
            lines[i + r].append(" ");
            for (auto j : pieces[i - 1])
            {
                int row, col;
                if (this_color == Utili::R)
                {
                    row = 10 - j / COL_COUNT - 1;
                    col = j % COL_COUNT;
                }
                else
                {
                    j = 89 - j;
                    row = 10 - j / COL_COUNT - 1;
                    col = j % COL_COUNT;
                }
                lines[i + r].append('<');
                lines[i + r].append(QString::number(col));
                lines[i + r].append(',');
                lines[i + r].append(QString::number(row));
                lines[i + r].append("> ");
            }
        }
        cout << 4 << endl;
        for (int i = 0; i < 7; i++)
            pieces[i].clear();
        cout << 5 << endl;
        for (auto cell : cells)
        {
            if (cell->get_chess_color() == Utili::B)
            {
                switch(cell->get_chess_piece())
                {
                case Utili::JIANGSHUAI:
                    pieces[0].push_back(cell->getId());
                    break;
                case Utili::SHI:
                    pieces[1].push_back(cell->getId());
                    break;
                case Utili::XIANG:
                    pieces[2].push_back(cell->getId());
                    break;
                case Utili::MA:
                    pieces[3].push_back(cell->getId());
                    break;
                case Utili::JU:
                    pieces[4].push_back(cell->getId());
                    break;
                case Utili::PAO:
                    pieces[5].push_back(cell->getId());
                    break;
                case Utili::ZUBING:
                    pieces[6].push_back(cell->getId());
                    break;
                default:
                    break;
                }
            }
        }
        for (int i = 1; i <= 7; i++)
        {
            lines[i + 8 - r].append(QString::number(pieces[i - 1].size()));
            lines[i + 8 - r].append(" ");
            for (auto j : pieces[i - 1])
            {
                int row, col;
                if (this_color == Utili::R)
                {
                    row = 10 - j / COL_COUNT - 1;
                    col = j % COL_COUNT;
                }
                else
                {
                    j = 89 - j;
                    row = 10 - j / COL_COUNT - 1;
                    col = j % COL_COUNT;
                }
                lines[i + 8 - r].append('<');
                lines[i + 8 - r].append(QString::number(col));
                lines[i + 8 - r].append(',');
                lines[i + 8 - r].append(QString::number(row));
                lines[i + 8 - r].append("> ");
            }
        }
        for (int i = 0; i < 16; i++)
            out << lines[i] << "\n";
        file->close();
    }
}

Utili::chess_color ChessBoard::get_this_color() const
{
    return this_color;
}

void ChessBoard::setReadWriteSocket(QTcpSocket *value, bool is_red)
{
    readWriteSocket = value;
    connect(readWriteSocket, SIGNAL(readyRead()), this, SLOT(opponent_move()));
    //new_game(is_red);
}

void ChessBoard::set_chess_piece(Utili::chess_type type, Utili::chess_color color, int p)
{
    cells[p]->set_chess_piece(type, color);
}

void ChessBoard::set_move(Utili::current_move _move)
{
    this->_move = _move;
}

Utili::current_move ChessBoard::get_move() const
{
    return _move;
}

void ChessBoard::receive_game(QByteArray str)
{
    clear_all();
    emit game_end();
    QTextStream stream(&str);
    Utili::chess_color cc;
    int c = 0;
    bool h = false;
    _red = false;
    this_color = Utili::B;
    QString line = stream.readLine();
    if (line.at(0) == 'r')
    {
        set_move(Utili::MOVE_OPPONENT);
        cc = Utili::R;
        c = 0;
    }
    else
    {
        set_move(Utili::MOVE_ME);
        cc = Utili::B;
        c = 0;
    }
    while (!stream.atEnd())
    {
        line = stream.readLine();
        //qDebug() << line;
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
                if (!_red)
                    index = 89 - index; // invert
                set_chess_piece(new_piece, cc, index);
            }
        }
    }
}
