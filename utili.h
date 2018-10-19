#ifndef UTILI_H
#define UTILI_H

#define ROW_COUNT 10
#define COL_COUNT 9
#define CELL_SUM 90
#define INDEX_TOP_LEFT 0
#define INDEX_TOP_RIGHT 8
#define INDEX_BOTTOM_LEFT 81
#define INDEX_BOTTOM_RIGHT 89
#define CELL_SIZE 80
#define INTERVAL 20

#include <QtCore>

class Utili
{
    Utili();
public:
    typedef enum { POS_TOP_LEFT, POS_TOP_RIGHT, POS_BOTTOM_LEFT, POS_BOTTOM_RIGHT,
                   POS_TOP, POS_LEFT, POS_BOTTOM, POS_RIGHT,
                   POS_PALACE_TOP_LEFT, POS_PALACE_TOP_RIGHT, POS_PALACE_BOTTOM_LEFT, POS_PALACE_BOTTOM_RIGHT,
                   POS_PALACE_CENTER, POS_PALACE_TOP_LEFT_TOP, POS_PALACE_TOP_RIGHT_TOP, POS_PALACE_BOTTOM_LEFT_BOTTOM, POS_PALACE_BOTTOM_RIGHT_BOTTOM,
                   POS_RIVER_TOP_EDGE, POS_RIVER_BOTTOM_EDGE,
                   POS_RIVER_TOP_EDGE_LEFT, POS_RIVER_TOP_EDGE_RIGHT, POS_RIVER_BOTTOM_EDGE_LEFT, POS_RIVER_BOTTOM_EDGE_RIGHT,
                   POS_NORMAL } chess_pos;
    typedef enum { R, B, NO_COLOR } chess_color;
    typedef enum { JU, MA, XIANG, SHI, JIANGSHUAI, ZUBING, NO_PIECE, PAO } chess_type;
    typedef enum { MOVE_ME, MOVE_OPPONENT } current_move;
    static QString get_str_chess_type(chess_type type);
    static char get_char_chess_type(chess_type type);
    static chess_type get_chess_type_from_char(char ch);
    static chess_color get_opposite_color(chess_color color);
    static chess_type get_chess_type_from_digit(int digit);
    //static QString get_line(int line);
};

#endif // UTILI_H
