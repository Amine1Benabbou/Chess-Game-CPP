#include "chessAi.h"
// void chessAi::load()
// {

//     int i = 0, j = 0;
//     FILE *f = fopen("save.txt", "r");
//     while (!feof(f))
//     {
//         int val = fgetc(f);

//         if (EOF != val)
//         {
//             if (val == 45)
//             {
//                 val = fgetc(f);
//                 board[i][j] = -(val - 48);
//             }
//             else
//             {
//                 board[i][j] = (val - 48);
//             }
//             j++;
//             if (j == 8)
//             {
//                 j = 0;
//                 i++;
//             }
//         }
//     }
//     fclose(f);
// }
chessAi::chessAi()
{
    // load();
}

chessAi::~chessAi()
{
}
// void chessAi::calculate(int &SW, int &SB)
// {
//     SW = 0;
//     SB = 0;
//     for (int i = 0; i < 8; i++)
//     {
//         for (int j = 0; j < 8; j++)
//         {
//             if (board[i][j] == 1)
//                 SW += pawnValue;
//             if (board[i][j] == 6)
//                 SW += rookValue;
//             if (board[i][j] == 5)
//                 SW += knightValue;
//             if (board[i][j] == 4)
//                 SW += bishopValue;
//             if (board[i][j] == 3)
//                 SW += kingValue;
//             if (board[i][j] == 2)
//                 SW += queenValue;
//             if (board[i][j] == -1)
//                 SB += pawnValue;
//             if (board[i][j] == -6)
//                 SB += rookValue;
//             if (board[i][j] == -5)
//                 SB += knightValue;
//             if (board[i][j] == -4)
//                 SB += bishopValue;
//             if (board[i][j] == -3)
//                 SB += kingValue;
//             if (board[i][j] == -2)
//                 SB += queenValue;
//         }
//     }
// }
// int chessAi::Evaluate(int piece)
// {
//     int whiteEval = 0;
//     int blackEval = 0;
//     int evaluation = whiteEval - blackEval;
//     // int perspective=(Board.WhiteToMove)?1:-1;
//     // return evalua
//     int SW, SB;
//     calculate(SW, SB);

//     if (piece > 0)
//         return SW - SB;
//     else if (piece < 0)
//         return SB - SW;
// }