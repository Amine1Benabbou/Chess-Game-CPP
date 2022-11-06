#ifndef CHESSAI_H
#define CHESSAI_H
#include <stdio.h>
#pragma once
struct moves
{
    int xd, yd, xa, ya;
};
class chessAi
{
public:
    chessAi();
    ~chessAi();
    // void calculate(int &, int &);
    // int Evaluate(int);
    // void load();

private:
    int board[8][8];

    const int pawnValue = 100;
    const int knightValue = 300;
    const int bishopValue = 300;
    const int rookValue = 500;
    const int kingValue = 600;
    const int queenValue = 900;
};

#endif