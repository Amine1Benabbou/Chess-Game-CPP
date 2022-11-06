#ifndef CHESS2_H_INCLUDED
#define CHESS2_H_INCLUDED

#include <string>
#include <vector>

typedef std::vector<std::string> buff;
typedef std::string str;

class Chess2
{
public:
    buff pseudomoves;
    buff legal_moves;
    short int board[8][8] = // This array represents the chess board
        {
            {-5, 0, 0, -6, -10, -2, -3, -5},
            {-1, -1, -1, 0, 0, -1, -1, -1},
            {0, 0, -3, -1, 0, 0, 0, 0},
            {0, 0, 0, 0, -1, 0, 0, 0},
            {0, 0, 2, 0, 1, 0, -2, 0},
            {0, 0, 3, 0, 0, 3, 0, 0},
            {1, 1, 1, 1, 0, 1, 1, 1},
            {5, 3, 2, 6, 10, 0, 0, 5},
    };
    int perform(str Move);
    str push(int row, int col, int desrow, int descol);
    buff getallmoves(bool turn);
    str computer_move(unsigned short int depth);
    bool checkmate(bool turn);

    bool check(bool turn);
    bool checkmatewhite = false;
    bool checkmateblack = false;

private:
    void getdiagonalmoves(bool turn, int row, int col);
    void getstraigtmoves(bool turn, int row, int col);
    void getknightmoves(bool turn, int row, int col);
    void getpawnmoves(bool turn, int row, int col);
    void getkingmoves(bool turn, int row, int col);
    int evaluation();
    int miniMax(int depth, bool ismax, int alpha, int beta);
    str miniMaxroot(int depth, bool turn);
    void undomove(int original, str Move);
};

#endif // CHESS2_H_INCLUDED