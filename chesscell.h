#ifndef CHESSCELL_H
#define CHESSCELL_H

#include <QWidget>
#include "utili.h"
#include <vector>
using namespace std;

class ChessCell : public QWidget
{
    Q_OBJECT
public:
    explicit ChessCell(QWidget *parent = nullptr);

    Utili::chess_pos get_cell_type() const;
    void set_cell_type(const Utili::chess_pos &value);
    void set_chess_piece(Utili::chess_type type, Utili::chess_color color);
    Utili::chess_type get_chess_piece() const;
    Utili::chess_color get_chess_color() const;
    vector<int> get_threatening_pos(bool target_only = false) const;

    int getId() const;
    void setId(int id);

    int get_row_num() const;
    void set_row_num(int row_num);

    int get_col_num() const;
    void set_col_num(int col_nsum);

    vector<ChessCell *> get_mates() const;

    void set_mates(const vector<ChessCell *> &value);
    void set_selected(bool t);

protected:
    void paintEvent(QPaintEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

signals:
    void mousePressed(ChessCell *p);

public slots:

private:
    Utili::chess_pos cell_type;
    Utili::chess_type _type;
    Utili::chess_color _color;
    int _id;
    int _row_num;
    int _col_num;
    vector<ChessCell *> mates;
    bool selected;
};

#endif // CHESSCELL_H
