#include "utili.h"

Utili::Utili()
{

}

QString Utili::get_str_chess_type(Utili::chess_type type)
{
    switch(type)
    {
    case Utili::JU:
        return "ju";
    case Utili::MA:
        return "ma";
    case Utili::XIANG:
        return "xiang";
    case Utili::SHI:
        return "shi";
    case Utili::JIANGSHUAI:
        return "jiangshuai";
    case Utili::ZUBING:
        return "zubing";
    case Utili::PAO:
        return "pao";
    }
}

char Utili::get_char_chess_type(Utili::chess_type type)
{
    switch(type)
    {
    case Utili::JU:
        return 'j';
    case Utili::MA:
        return 'm';
    case Utili::XIANG:
        return 'x';
    case Utili::SHI:
        return 's';
    case Utili::JIANGSHUAI:
        return 'w';
    case Utili::ZUBING:
        return 'B';
    case Utili::PAO:
        return 'p';
    }
}

Utili::chess_type Utili::get_chess_type_from_char(char ch)
{
    switch(ch)
    {
    case 'j':
        return Utili::JU;
    case 'm':
        return Utili::MA;
    case 'x':
        return Utili::XIANG;
    case 's':
        return Utili::SHI;
    case 'w':
        return Utili::JIANGSHUAI;
    case 'B':
        return Utili::ZUBING;
    case 'p':
        return Utili::PAO;
    }
}

Utili::chess_color Utili::get_opposite_color(Utili::chess_color color)
{
    switch(color)
    {
    case R:
        return B;
    case B:
        return R;
    case NO_COLOR:
        return NO_COLOR;
    }
}

Utili::chess_type Utili::get_chess_type_from_digit(int digit)
{
    switch(digit)
    {
    case 1:
        return JIANGSHUAI;
    case 2:
        return SHI;
    case 3:
        return XIANG;
    case 4:
        return MA;
    case 5:
        return JU;
    case 6:
        return PAO;
    case 7:
        return ZUBING;
    }
}
