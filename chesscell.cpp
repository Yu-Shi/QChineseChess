#include "chesscell.h"
#include <QPaintEvent>
#include <QPainter>
#include <iostream>
#include <QMouseEvent>
using namespace std;

ChessCell::ChessCell(QWidget *parent) : QWidget(parent), _type(Utili::NO_PIECE), _color(Utili::NO_COLOR), selected(false)
{
    setFixedSize(CELL_SIZE, CELL_SIZE);
}

Utili::chess_pos ChessCell::get_cell_type() const
{
    return cell_type;
}

void ChessCell::set_cell_type(const Utili::chess_pos &value)
{
    cell_type = value;
}

void ChessCell::set_chess_piece(Utili::chess_type type, Utili::chess_color color)
{
    _type = type;
    _color = color;
    update();
}

Utili::chess_type ChessCell::get_chess_piece() const
{
    return _type;
}

Utili::chess_color ChessCell::get_chess_color() const
{
    return _color;
}

vector<int> ChessCell::get_threatening_pos(bool target_only) const
{
    vector<int> return_vector;
    int t;
    int flag = 0;
    switch(_type)
    {
    case Utili::JU:

        for (int i = _row_num * COL_COUNT + _col_num + 1; i < _row_num * COL_COUNT + 9; i++)
        {
            return_vector.push_back(i);
            if (mates[i]->get_chess_piece() != Utili::NO_PIECE)
                break;
        }
        for (int i = _row_num * COL_COUNT + _col_num - 1; i >= _row_num * COL_COUNT; i--)
        {
            return_vector.push_back(i);
            if (mates[i]->get_chess_piece() != Utili::NO_PIECE)
                break;
        }
        for (int i = _id + COL_COUNT; i <= 9 * COL_COUNT + _col_num; i += COL_COUNT)
        {
            return_vector.push_back(i);
            if (mates[i]->get_chess_piece() != Utili::NO_PIECE)
                break;
        }
        for (int i = _id - COL_COUNT; i >= _col_num; i -= COL_COUNT)
        {
            return_vector.push_back(i);
            if (mates[i]->get_chess_piece() != Utili::NO_PIECE)
                break;
        }
        return return_vector;

    case Utili::MA:

        t = _id - 2 * COL_COUNT - 1;
        if (t >= 0 && _col_num >= 1 && mates[_id - COL_COUNT]->get_chess_piece() == Utili::NO_PIECE) // "bie ma jiao" considered.
            return_vector.push_back(t);

        t = _id - 2 * COL_COUNT + 1;
        if (t >= 0 && _col_num < COL_COUNT - 1 && mates[_id - COL_COUNT]->get_chess_piece() == Utili::NO_PIECE)
            return_vector.push_back(t);

        t = _id - COL_COUNT - 2;
        if (t >= 0 && _col_num >= 2 && mates[_id - 1]->get_chess_piece() == Utili::NO_PIECE)
            return_vector.push_back(t);

        t = _id - COL_COUNT + 2;
        if (t >= 0 && _col_num < COL_COUNT - 2 && mates[_id + 1]->get_chess_piece() == Utili::NO_PIECE)
            return_vector.push_back(t);

        t = _id + 2 * COL_COUNT - 1;
        if (t <= INDEX_BOTTOM_RIGHT && _col_num >= 1 && mates[_id + COL_COUNT]->get_chess_piece() == Utili::NO_PIECE)
            return_vector.push_back(t);

        t = _id + 2 * COL_COUNT + 1;
        if (t <= INDEX_BOTTOM_RIGHT && _col_num < COL_COUNT - 1 && mates[_id + COL_COUNT]->get_chess_piece() == Utili::NO_PIECE)
            return_vector.push_back(t);

        t = _id + COL_COUNT - 2;
        if (t <= INDEX_BOTTOM_RIGHT && _col_num >= 2 && mates[_id - 1]->get_chess_piece() == Utili::NO_PIECE)
            return_vector.push_back(t);

        t = _id + COL_COUNT + 2;
        if (t <= INDEX_BOTTOM_RIGHT && _col_num < COL_COUNT - 2 && mates[_id + 1]->get_chess_piece() == Utili::NO_PIECE)
            return_vector.push_back(t);

        return return_vector;

    case Utili::XIANG:

        t = _id - 2 * COL_COUNT - 2;
        if (t >= 0 && _col_num >= 2 && mates[_id - COL_COUNT - 1]->get_chess_piece() == Utili::NO_PIECE && _row_num != 5)
            return_vector.push_back(t);

        t = _id - 2 * COL_COUNT + 2;
        if (t >= 0 && _col_num < COL_COUNT - 2 && mates[_id - COL_COUNT + 1]->get_chess_piece() == Utili::NO_PIECE && _row_num != 5)
            return_vector.push_back(t);

        t = _id + 2 * COL_COUNT - 2;
        if (t <= INDEX_BOTTOM_RIGHT && _col_num >= 2 && mates[_id + COL_COUNT - 1]->get_chess_piece() == Utili::NO_PIECE)
            return_vector.push_back(t);

        t = _id + 2 * COL_COUNT + 2;
        if (t <= INDEX_BOTTOM_RIGHT && _col_num < COL_COUNT - 2 && mates[_id + COL_COUNT + 1]->get_chess_piece() == Utili::NO_PIECE)
            return_vector.push_back(t);

        return return_vector;

    case Utili::SHI:

        if (cell_type == Utili::POS_PALACE_BOTTOM_LEFT ||
            cell_type == Utili::POS_PALACE_BOTTOM_RIGHT ||
            cell_type == Utili::POS_PALACE_TOP_LEFT_TOP ||
            cell_type == Utili::POS_PALACE_TOP_RIGHT_TOP)
        {
            return_vector.push_back(13);
            return return_vector;
        }
        else if (cell_type == Utili::POS_PALACE_TOP_LEFT ||
                 cell_type == Utili::POS_PALACE_TOP_RIGHT ||
                 cell_type == Utili::POS_PALACE_BOTTOM_LEFT_BOTTOM ||
                 cell_type == Utili::POS_PALACE_BOTTOM_RIGHT_BOTTOM)
        {
            return_vector.push_back(76);
            return return_vector;
        }
        else if (_id == 13)
        {
            return_vector.push_back(13 - COL_COUNT - 1);
            return_vector.push_back(13 - COL_COUNT + 1);
            return_vector.push_back(13 + COL_COUNT - 1);
            return_vector.push_back(13 + COL_COUNT + 1);
            return return_vector;
        }
        else if (_id == 76)
        {
            return_vector.push_back(76 - COL_COUNT - 1);
            return_vector.push_back(76 - COL_COUNT + 1);
            return_vector.push_back(76 + COL_COUNT - 1);
            return_vector.push_back(76 + COL_COUNT + 1);
            return return_vector;
        }

    case Utili::JIANGSHUAI:

        for (int i = _id - COL_COUNT; i >= 0; i -= COL_COUNT)
        {
            if (mates[i]->get_chess_piece() == Utili::JIANGSHUAI)
            {
                flag = i;
                break;
            }
            else if (mates[i]->get_chess_piece() != Utili::NO_PIECE)
            {
                flag = 0;
                break;
            }
        }
        if (flag)
        {
            return_vector.push_back(flag);
            cout << flag << endl;
            cout << "hello4343" << std::endl;
        }
        flag = 0;
        if (_col_num == 4)
        {
            for (int i = _id - 1 - COL_COUNT; i >= 0; i -= COL_COUNT)
            {
                if (mates[i]->get_chess_piece() == Utili::JIANGSHUAI)
                {
                    flag = 1;
                    break;
                }
                else if (mates[i]->get_chess_piece() != Utili::NO_PIECE)
                {
                    flag = 0;
                    break;
                }
            }
            if (!flag)
                return_vector.push_back(_id - 1);
            flag = 0;
            for (int i = _id + 1 - COL_COUNT; i >= 0; i -= COL_COUNT)
            {
                if (mates[i]->get_chess_piece() == Utili::JIANGSHUAI)
                {
                    flag = 1;
                    break;
                }
                else if (mates[i]->get_chess_piece() != Utili::NO_PIECE)
                {
                    flag = 0;
                    break;
                }
            }
            if (!flag)
                return_vector.push_back(_id + 1);
            flag = 0;
        }
        else if (_col_num == 3)
        {
            for (int i = _id + 1 - COL_COUNT; i >= 0; i -= COL_COUNT)
            {
                if (mates[i]->get_chess_piece() == Utili::JIANGSHUAI)
                {
                    flag = 1;
                    break;
                }
                else if (mates[i]->get_chess_piece() != Utili::NO_PIECE)
                {
                    flag = 0;
                    break;
                }
            }
            if (!flag)
                return_vector.push_back(_id + 1);
            flag = 0;
        }
        else if (_col_num == 5)
        {
            for (int i = _id - 1 - COL_COUNT; i >= 0; i -= COL_COUNT)
            {
                if (mates[i]->get_chess_piece() == Utili::JIANGSHUAI)
                {
                    flag = 1;
                    break;
                }
                else if (mates[i]->get_chess_piece() != Utili::NO_PIECE)
                {
                    flag = 0;
                    break;
                }
            }
            if (!flag)
                return_vector.push_back(_id - 1);
            flag = 0;
        }
        if (_row_num == 1 || _row_num == 2)
            return_vector.push_back(_id - COL_COUNT);
        if (_row_num == 0 || _row_num == 1)
            return_vector.push_back(_id + COL_COUNT);
        if (_row_num == 7 || _row_num == 8)
            return_vector.push_back(_id + COL_COUNT);
        if (_row_num == 8 || _row_num == 9)
            return_vector.push_back(_id - COL_COUNT);
        return return_vector;

    case Utili::ZUBING:

        if (_id - COL_COUNT >= 0)
            return_vector.push_back(_id - COL_COUNT);
        if (_row_num <= 4 && _col_num > 0)
            return_vector.push_back(_id - 1);
        if (_row_num <= 4 && _col_num < COL_COUNT - 1)
            return_vector.push_back(_id + 1);
        return return_vector;

    case Utili::PAO:

        int psame = -1;
        for (int i = _row_num * COL_COUNT + _col_num + 1; i < _row_num * COL_COUNT + 9; i++)
        {
            if (psame != -1)
            {
                if (mates[i]->get_chess_color() == Utili::get_opposite_color(_color))
                {
                    return_vector.push_back(i);
                    break;
                }
            }
            else
            {
                if (!target_only && mates[i]->get_chess_piece() == Utili::NO_PIECE)
                    return_vector.push_back(i);
            }
            if (mates[i]->get_chess_piece() != Utili::NO_PIECE) // "pao jia"
            {
                psame = i;
            }
        }
        psame = -1;
        for (int i = _row_num * COL_COUNT + _col_num - 1; i >= _row_num * COL_COUNT; i--)
        {
            if (psame != -1)
            {
                if (mates[i]->get_chess_color() == Utili::get_opposite_color(_color))
                {
                    return_vector.push_back(i);
                    break;
                }
            }
            else
            {
                if (!target_only && mates[i]->get_chess_piece() == Utili::NO_PIECE)
                    return_vector.push_back(i);
            }
            if (mates[i]->get_chess_piece() != Utili::NO_PIECE) // "pao jia"
            {
                psame = i;
            }
        }
        psame = -1;
        for (int i = _id + COL_COUNT; i <= 9 * COL_COUNT + _col_num; i += COL_COUNT)
        {
            if (psame != -1)
            {
                if (mates[i]->get_chess_color() == Utili::get_opposite_color(_color))
                {
                    return_vector.push_back(i);
                    break;
                }
            }
            else
            {
                if (!target_only && mates[i]->get_chess_piece() == Utili::NO_PIECE)
                    return_vector.push_back(i);
            }
            if (mates[i]->get_chess_piece() != Utili::NO_PIECE) // "pao jia"
            {
                psame = i;
            }
        }
        psame = -1;
        for (int i = _id - COL_COUNT; i >= _col_num; i -= COL_COUNT)
        {
            if (psame != -1)
            {
                if (mates[i]->get_chess_color() == Utili::get_opposite_color(_color))
                {
                    return_vector.push_back(i);
                    break;
                }
            }
            else
            {
                if (!target_only && mates[i]->get_chess_piece() == Utili::NO_PIECE)
                    return_vector.push_back(i);
            }
            if (mates[i]->get_chess_piece() != Utili::NO_PIECE) // "pao jia"
            {
                psame = i;
            }
        }
        return return_vector;
    }
}

void ChessCell::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    QPen pen_thick(Qt::red);
    QPen pen_normal(Qt::black);
    QPen pen_medium(Qt::black);
    QPen pen_dot(Qt::black);
    pen_thick.setWidth(5);
    pen_normal.setWidth(2);
    pen_normal.setCapStyle(Qt::FlatCap);
    pen_medium.setWidth(2);
    pen_normal.setCapStyle(Qt::SquareCap);
    pen_dot.setWidth(2);
    pen_dot.setStyle(Qt::DashDotDotLine);
    int w = this->width();
    int h = this->height();
    switch(cell_type)
    {
    case Utili::POS_TOP_LEFT:
        p.setPen(pen_thick);
        p.drawLine(w * 0.2, h * 0.2, w * 0.2, h);
        p.drawLine(w * 0.2, h * 0.2, w, h * 0.2);
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, h * 0.5, w * 0.5, h);
        p.drawLine(w * 0.5, h * 0.5, w, h * 0.5);
        break;
    case Utili::POS_TOP_RIGHT:
        p.setPen(pen_thick);
        p.drawLine(w * 0.8, h * 0.2, w * 0.8, h);
        p.drawLine(0, h * 0.2, w * 0.8, h * 0.2);
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, h * 0.5, w * 0.5, h);
        p.drawLine(0, h * 0.5, w * 0.5, h * 0.5);
        break;
    case Utili::POS_BOTTOM_LEFT:
        p.setPen(pen_thick);
        p.drawLine(w * 0.2, 0, w * 0.2, h * 0.8);
        p.drawLine(w * 0.2, h * 0.8, w, h * 0.8);
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, 0, w * 0.5, h * 0.5);
        p.drawLine(w * 0.5, h * 0.5, w, h * 0.5);
        break;
    case Utili::POS_BOTTOM_RIGHT:
        p.setPen(pen_thick);
        p.drawLine(w * 0.8, 0, w * 0.8, h * 0.8);
        p.drawLine(0, h * 0.8, w * 0.8, h * 0.8);
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, 0, w * 0.5, h * 0.5);
        p.drawLine(0, h * 0.5, w * 0.5, h * 0.5);
        break;
    case Utili::POS_NORMAL:
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, 0, w * 0.5, h);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        break;
    case Utili::POS_TOP:
        p.setPen(pen_thick);
        p.drawLine(0, h * 0.2, w, h * 0.2);
        p.setPen(pen_normal);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.drawLine(w * 0.5, h * 0.5, w * 0.5, h);
        break;
    case Utili::POS_BOTTOM:
        p.setPen(pen_thick);
        p.drawLine(0, h * 0.8, w, h * 0.8);
        p.setPen(pen_normal);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.drawLine(w * 0.5, 0, w * 0.5, h * 0.5);
        break;
    case Utili::POS_LEFT:
        p.setPen(pen_thick);
        p.drawLine(w * 0.2, 0, w * 0.2, h);
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, 0, w * 0.5, h);
        p.drawLine(w * 0.5, h * 0.5, w, h * 0.5);
        break;
    case Utili::POS_RIGHT:
        p.setPen(pen_thick);
        p.drawLine(w * 0.8, 0, w * 0.8, h);
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, 0, w * 0.5, h);
        p.drawLine(0, h * 0.5, w * 0.5, h * 0.5);
        break;
    case Utili::POS_PALACE_TOP_LEFT:
        p.setPen(pen_normal);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.drawLine(w * 0.5, 0, w * 0.5, h);
        p.setPen(pen_dot);
        p.drawLine(w * 0.5, h * 0.5, w, h);
        break;
    case Utili::POS_PALACE_TOP_LEFT_TOP:
        p.setPen(pen_thick);
        p.drawLine(0, h * 0.2, w, h * 0.2);
        p.setPen(pen_normal);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.drawLine(w * 0.5, h * 0.5, w * 0.5, h);
        p.setPen(pen_dot);
        p.drawLine(w * 0.5, h * 0.5, w, h);
        break;
    case Utili::POS_PALACE_TOP_RIGHT:
        p.setPen(pen_normal);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.drawLine(w * 0.5, 0, w * 0.5, h);
        p.setPen(pen_dot);
        p.drawLine(w * 0.5, h * 0.5, 0, h);
        break;
    case Utili::POS_PALACE_TOP_RIGHT_TOP:
        p.setPen(pen_thick);
        p.drawLine(0, h * 0.2, w, h * 0.2);
        p.setPen(pen_normal);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.drawLine(w * 0.5, h * 0.5, w * 0.5, h);
        p.setPen(pen_dot);
        p.drawLine(w * 0.5, h * 0.5, 0, h);
        break;
    case Utili::POS_PALACE_BOTTOM_LEFT:
        p.setPen(pen_normal);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.drawLine(w * 0.5, 0, w * 0.5, h);
        p.setPen(pen_dot);
        p.drawLine(w * 0.5, h * 0.5, w, 0);
        break;
    case Utili::POS_PALACE_BOTTOM_LEFT_BOTTOM:
        p.setPen(pen_thick);
        p.drawLine(0, h * 0.8, w, h * 0.8);
        p.setPen(pen_normal);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.drawLine(w * 0.5, h * 0.5, w * 0.5, 0);
        p.setPen(pen_dot);
        p.drawLine(w * 0.5, h * 0.5, w, 0);
        break;
    case Utili::POS_PALACE_BOTTOM_RIGHT:
        p.setPen(pen_normal);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.drawLine(w * 0.5, 0, w * 0.5, h);
        p.setPen(pen_dot);
        p.drawLine(w * 0.5, h * 0.5, 0, 0);
        break;
    case Utili::POS_PALACE_BOTTOM_RIGHT_BOTTOM:
        p.setPen(pen_thick);
        p.drawLine(0, h * 0.8, w, h * 0.8);
        p.setPen(pen_normal);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.drawLine(w * 0.5, h * 0.5, w * 0.5, 0);
        p.setPen(pen_dot);
        p.drawLine(w * 0.5, h * 0.5, 0, 0);
        break;
    case Utili::POS_RIVER_TOP_EDGE:
        p.setPen(pen_medium);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, 0, w * 0.5, h * 0.5);
        break;
    case Utili::POS_RIVER_BOTTOM_EDGE:
        p.setPen(pen_medium);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, h * 0.5, w * 0.5, h);
        break;
    case Utili::POS_RIVER_TOP_EDGE_LEFT:
        p.setPen(pen_thick);
        p.drawLine(w * 0.2, 0, w * 0.2, h);
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, 0, w * 0.5, h);
        p.setPen(pen_medium);
        p.drawLine(w * 0.5, h * 0.5, w, h * 0.5);
        break;
    case Utili::POS_RIVER_BOTTOM_EDGE_LEFT:
        p.setPen(pen_thick);
        p.drawLine(w * 0.2, 0, w * 0.2, h);
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, 0, w * 0.5, h);
        p.setPen(pen_medium);
        p.drawLine(w * 0.5, h * 0.5, w, h * 0.5);
        break;
    case Utili::POS_RIVER_TOP_EDGE_RIGHT:
    case Utili::POS_RIVER_BOTTOM_EDGE_RIGHT:
        p.setPen(pen_thick);
        p.drawLine(w * 0.8, 0, w * 0.8, h);
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, 0, w * 0.5, h);
        p.setPen(pen_medium);
        p.drawLine(0, h * 0.5, w * 0.5, h * 0.5);
        break;
    case Utili::POS_PALACE_CENTER:
        p.setPen(pen_normal);
        p.drawLine(w * 0.5, 0, w * 0.5, h);
        p.drawLine(0, h * 0.5, w, h * 0.5);
        p.setPen(pen_dot);
        p.drawLine(0, 0, w, h);
        p.drawLine(0, h, w, 0);
        break;
    //default:
    }
    if (_type != Utili::NO_PIECE)
    {
        QImage image;
        QString str;
        str = QString(":/pieces/") + ((_color == Utili::R) ? "r" : "b") + QString("_") + Utili::get_str_chess_type(_type);
        image.load(str);
        p.drawImage(rect().adjusted(3, 3, -3, -3), image);
    }
    if (selected)
    {
        pen_normal.setColor(Qt::blue);
        p.setPen(pen_normal);
        p.drawLine(0, 0, w * 0.3, 0);
        p.drawLine(w * 0.7, 0, w, 0);
        p.drawLine(0, 0, 0, h * 0.3);
        p.drawLine(0, h * 0.7, 0, h);
        p.drawLine(w, 0, w, h * 0.3);
        p.drawLine(w, h * 0.7, w, h);
        p.drawLine(0, h, w * 0.3, h);
        p.drawLine(w * 0.7, h, w, h);
    }
}

void ChessCell::mouseReleaseEvent(QMouseEvent *)
{
    cout << "pressed" << endl;
    //set_selected();
    emit mousePressed(this);
}

void ChessCell::set_mates(const vector<ChessCell *> &value)
{
    mates = value;
}

void ChessCell::set_selected(bool t)
{
    selected = t;
    update();
}

vector<ChessCell *> ChessCell::get_mates() const
{
    return mates;
}

int ChessCell::get_col_num() const
{
    return _col_num;
}

void ChessCell::set_col_num(int col_nsum)
{
    _col_num = col_nsum;
}

int ChessCell::get_row_num() const
{
    return _row_num;
}

void ChessCell::set_row_num(int row_num)
{
    _row_num = row_num;
}

int ChessCell::getId() const
{
    return _id;
}

void ChessCell::setId(int id)
{
    _id = id;
}
