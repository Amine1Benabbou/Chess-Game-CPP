#include "chessAi.cpp"
#include "include/raylib.h"
#include "include/raymath.h"
#include <stdio.h>

#include <string.h>
using namespace std;
const int screenWidth = 452;
const int screenHeight = 452;
struct AnimData
{
	Rectangle rec;
};
struct Chess_pieces
{
	int Pawn{1}, Knight{5}, bishop{4}, rook{6}, queen{3}, king{2};
};
Chess_pieces pieces;
int CheckForCheckmateBoard[8][8];
int board[8][8] =
	{-pieces.rook, -pieces.Knight, -pieces.bishop, -pieces.queen, -pieces.king, -pieces.bishop, -pieces.Knight, -pieces.rook,
	 -pieces.Pawn, -pieces.Pawn, -pieces.Pawn, -pieces.Pawn, -pieces.Pawn, -pieces.Pawn, -pieces.Pawn, -pieces.Pawn,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 pieces.Pawn, pieces.Pawn, pieces.Pawn, pieces.Pawn, pieces.Pawn, pieces.Pawn, pieces.Pawn, pieces.Pawn,
	 pieces.rook, pieces.Knight, pieces.bishop, pieces.queen, pieces.king, pieces.bishop, pieces.Knight, pieces.rook};
int resetboard[8][8] =
	{-pieces.rook, -pieces.Knight, -pieces.bishop, -pieces.queen, -pieces.king, -pieces.bishop, -pieces.Knight, -pieces.rook,
	 -pieces.Pawn, -pieces.Pawn, -pieces.Pawn, -pieces.Pawn, -pieces.Pawn, -pieces.Pawn, -pieces.Pawn, -pieces.Pawn,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 pieces.Pawn, pieces.Pawn, pieces.Pawn, pieces.Pawn, pieces.Pawn, pieces.Pawn, pieces.Pawn, pieces.Pawn,
	 pieces.rook, pieces.Knight, pieces.bishop, pieces.queen, pieces.king, pieces.bishop, pieces.Knight, pieces.rook};

bool menu_dame_black{}, menu_dame_white{}, lockmvt{};
void Drow_figures(Texture2D figures, AnimData TfiguresBlack[], AnimData Tfigures[])
{

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			int n = board[i][j];
			if (n < 0)
			{
				DrawTextureRec(figures, TfiguresBlack[abs(board[i][j]) - 1].rec, {(float)j * screenWidth / 8 - 4, (float)i * screenHeight / 8}, WHITE);
			}
			else if (n > 0)
			{
				DrawTextureRec(figures, Tfigures[abs(board[i][j]) - 1].rec, {(float)j * screenWidth / 8 - 4, (float)i * screenHeight / 8 - 5}, WHITE);
			}
		}
}

bool freindly{}, kingInCheck{};

// Fonctions
// place BLANC Ligne down
int placeXd(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd + cp; i < xa; i++)
	{
		if (board[xd + cp][yd] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			i = xa;
		}
	}
	return cp;
}

// place Noir Ligne down
int placeXnd(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd + cp; i < xa; i++)
	{
		if (board[xd + cp][yd] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			i = xa;
		}
	}
	return cp;
}

// place PAWN Ligne up
int placeXPu(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd - cp; i >= xa; i--)
	{
		if (board[xd - cp][yd] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			i = xa;
		}
	}
	return cp;
}

// place PAWN Ligne down
int placeXPd(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd + cp; i <= xa; i++)
	{
		if (board[xd + cp][yd] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			i = xa;
		}
	}
	return cp;
}

// place diagdown à la droite BLANC
int placeXDd(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd + cp, j = yd + cp; i < xa && j < ya; i++, j++)
	{
		if (board[xd + cp][yd + cp] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			break;
		}
	}
	return cp;
}
// place diagdown à la droite NOIR
int placeXnDd(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd + cp, j = yd + cp; i < xa && j < ya; i++, j++)
	{
		if (board[xd + cp][yd + cp] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			break;
		}
	}
	return cp;
}

// place diagdown à gauche BLANC
int placeXGd(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd + cp, j = yd - cp; i < xa && j > ya; i++, j--)
	{
		if (board[xd + cp][yd - cp] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			break;
		}
	}
	return cp;
}

// place diagdown à gauche NOIR
int placeXnGd(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd + cp, j = yd - cp; i < xa && j > ya; i++, j--)
	{
		if (board[xd + cp][yd - cp] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			break;
		}
	}
	return cp;
}

// place diagUp à la droite BLANC
int placeXDu(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd - cp, j = yd + cp; i > xa && j < ya; i--, j++)
	{
		if (board[xd - cp][yd + cp] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			break;
		}
	}
	return cp;
}

// place diagUp à la droite NOIR
int placeXnDu(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd - cp, j = yd + cp; i > xa && j < ya; i--, j++)
	{
		if (board[xd - cp][yd + cp] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			break;
		}
	}
	return cp;
}

// place diagUp à gauche BLANC
int placeXGu(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd - cp, j = yd - cp; i > xa && j > ya; i--, j--)
	{
		if (board[xd - cp][yd - cp] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			break;
		}
	}
	return cp;
}

// place diagUp à gauche NOIR
int placeXnGu(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd - cp, j = yd - cp; i > xa && j > ya; i--, j--)
	{
		if (board[xd - cp][yd - cp] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			break;
		}
	}
	return cp;
}

// place BLANC Ligne up
int placeXu(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd - cp; i > xa; i--)
	{
		if (board[xd - cp][yd] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			i = xa;
		}
	}
	return cp;
}

// place Noir Ligne up
int placeXnu(int xd, int xa, int yd, int ya)
{
	int cp = 1;

	for (int i = xd - cp; i > xa; i--)
	{
		if (board[xd - cp][yd] == 0)
		{
			cp++;
		}
		else
		{
			cp = 0;
			i = xa;
		}
	}
	return cp;
}

// place BLANC colonne droite
int placeYd(int xd, int xa, int yd, int ya)
{
	int cd = 1;

	for (int i = yd + cd; i < ya; i++)
	{
		if (board[xd][yd + cd] == 0)
		{
			cd++;
		}
		else
		{
			cd = 0;
			i = ya;
		}
	}
	return cd;
}

// place Noir colonne droite
int placeYnd(int xd, int xa, int yd, int ya)
{
	int cd = 1;

	for (int i = yd + cd; i < ya; i++)
	{
		if (board[xd][yd + cd] == 0)
		{
			cd++;
		}
		else
		{
			cd = 0;
			i = ya;
		}
	}
	return cd;
}

// place BLANC colonne gauche
int placeYg(int xd, int xa, int yd, int ya)
{
	int cd = 1;

	for (int i = yd - cd; i > ya; i--)
	{
		if (board[xd][yd - cd] == 0)
		{
			cd++;
		}
		else
		{
			cd = 0;
			i = ya;
		}
	}
	return cd;
}

// place Noir colonne gauche
int placeYng(int xd, int xa, int yd, int ya)
{
	int cd = 1;

	for (int i = yd - cd; i > ya; i--)
	{
		if (board[xd][yd - cd] == 0)
		{
			cd++;
		}
		else
		{
			cd = 0;
			i = ya;
		}
	}
	return cd;
}
//--------------------------------------------------------------------------------------------
void Drow_possible_figures_moves(int piece, Texture2D AttackPiece, Texture2D selection, AnimData Tselection, Texture2D SelectionPiece, AnimData tSelectionPiece, int xd, int yd, int ya, int xa)
{
	Vector2 selection_pos, step;
	selection_pos.x = (float)xd * screenWidth / 8;
	selection_pos.y = (float)yd * screenHeight / 8 + 2;
	if (xd < 2 && yd < 2)
	{
		selection_pos.x = (float)xd * screenWidth / 8 + 1;
		selection_pos.y = (float)yd * screenHeight / 8 + 2;
	}
	else if (xd < 2 && yd < 4)
	{
		selection_pos.x = (float)xd * screenWidth / 8 + 1;
		selection_pos.y = (float)yd * screenHeight / 8 + 1;
	}
	else if (xd < 2 && yd < 6)
	{
		selection_pos.x = (float)xd * screenWidth / 8 + 1;
		selection_pos.y = (float)yd * screenHeight / 8;
	}
	else if (xd < 2 && yd < 8)
	{
		selection_pos.x = (float)xd * screenWidth / 8 + 1;
		selection_pos.y = (float)yd * screenHeight / 8 - 1;
	}
	else if (xd < 4 && yd < 2)
	{
		selection_pos.x = (float)xd * screenWidth / 8;
		selection_pos.y = (float)yd * screenHeight / 8 + 2;
	}
	else if (xd < 4 && yd < 4)
	{
		selection_pos.x = (float)xd * screenWidth / 8;
		selection_pos.y = (float)yd * screenHeight / 8 + 1;
	}
	else if (xd < 4 && yd < 6)
	{
		selection_pos.x = (float)xd * screenWidth / 8;
		selection_pos.y = (float)yd * screenHeight / 8;
	}
	else if (xd < 4 && yd < 8)
	{
		selection_pos.x = (float)xd * screenWidth / 8;
		selection_pos.y = (float)yd * screenHeight / 8 - 1;
	}
	else if (xd < 6 && yd < 2)
	{
		selection_pos.x = (float)xd * screenWidth / 8 - 1;
		selection_pos.y = (float)yd * screenHeight / 8 + 2;
	}
	else if (xd < 6 && yd < 4)
	{
		selection_pos.x = (float)xd * screenWidth / 8 - 1;
		selection_pos.y = (float)yd * screenHeight / 8 + 1;
	}
	else if (xd < 6 && yd < 6)
	{
		selection_pos.x = (float)xd * screenWidth / 8 - 1;
		selection_pos.y = (float)yd * screenHeight / 8;
	}
	else if (xd < 6 && yd < 8)
	{
		selection_pos.x = (float)xd * screenWidth / 8 - 1;
		selection_pos.y = (float)yd * screenHeight / 8 - 1;
	}
	else if (xd < 8 && yd < 2)
	{
		selection_pos.x = (float)xd * screenWidth / 8 - 2;
		selection_pos.y = (float)yd * screenHeight / 8 + 2;
	}
	else if (xd < 8 && yd < 4)
	{
		selection_pos.x = (float)xd * screenWidth / 8 - 2;
		selection_pos.y = (float)yd * screenHeight / 8 + 1;
	}
	else if (xd < 8 && yd < 6)
	{
		selection_pos.x = (float)xd * screenWidth / 8 - 2;
		selection_pos.y = (float)yd * screenHeight / 8;
	}
	else if (xd < 8 && yd < 8)
	{
		selection_pos.x = (float)xd * screenWidth / 8 - 2;
		selection_pos.y = (float)yd * screenHeight / 8 - 1;
	}
	step.x = selection_pos.x;
	step.y = selection_pos.y;

	if (!kingInCheck)
	{

		// Pawn:
		// blanc
		if ((piece == -pieces.Pawn))
		{

			step.y = selection_pos.y + screenWidth / 8;
			DrawTextureRec(SelectionPiece, tSelectionPiece.rec, selection_pos, WHITE);

			if (board[yd + 1][xd] == 0)
			{
				DrawTextureRec(selection, Tselection.rec, step, WHITE);
				if (yd == 1)
				{
					step.y = step.y + screenWidth / 8;

					DrawTextureRec(selection, Tselection.rec, step, WHITE);
				}
			}

			if (board[yd + 1][xd + 1] > 0)
			{
				step.y = selection_pos.y + screenWidth / 8;
				step.x = selection_pos.x + screenWidth / 8;
				DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, RED);
			}

			if (board[yd + 1][xd - 1] > 0)
			{
				step.y = selection_pos.y + screenWidth / 8;
				step.x = selection_pos.x - screenWidth / 8;
				DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, RED);
			}
		}

		// noir
		if ((piece == pieces.Pawn))
		{
			step.y = step.y - screenWidth / 8;
			DrawTextureRec(SelectionPiece, tSelectionPiece.rec, selection_pos, WHITE);
			if (board[yd - 1][xd] == 0)
			{
				DrawTextureRec(selection, Tselection.rec, step, WHITE);
				if (yd == 6)
				{
					step.y = step.y - screenWidth / 8;
					DrawTextureRec(selection, Tselection.rec, step, WHITE);
				}
			}
			if (board[yd - 1][xd + 1] < 0)
			{
				step.y = selection_pos.y - screenWidth / 8;
				step.x = selection_pos.x + screenWidth / 8;
				DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, RED);
			}

			if (board[yd - 1][xd - 1] < 0)
			{
				step.y = selection_pos.y - screenWidth / 8;
				step.x = selection_pos.x - screenWidth / 8;
				DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, RED);
			}
		}

		//----------------------------------------------------------
		// Knight:
		if ((piece == pieces.Knight) || (piece == -pieces.Knight))
		{
			step.y = step.y - 2 * screenWidth / 8;
			step.x = step.x - 2 * screenWidth / 8;
			DrawTextureRec(SelectionPiece, tSelectionPiece.rec, selection_pos, WHITE);
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					if ((i % 2 == 0 && j % 2 != 0 && i != 2) || (j % 2 == 0 && i % 2 != 0 && j != 2))
					{

						if (piece == pieces.Knight) //|| ((piece == -pieces.Knight) && (board[i][j] > 0))
						{
							Vector2 pos = selection_pos;

							if (board[yd - 2][xd - 1] < 0)
							{
								pos.x = selection_pos.x - screenWidth / 8;
								pos.y = selection_pos.y - 2 * screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd - 2][xd + 1] < 0)
							{
								pos.x = selection_pos.x + screenWidth / 8;
								pos.y = selection_pos.y - 2 * screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd + 2][xd - 1] < 0)
							{
								pos.x = selection_pos.x - screenWidth / 8;
								pos.y = selection_pos.y + 2 * screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd + 2][xd + 1] < 0)
							{
								pos.x = selection_pos.x + screenWidth / 8;
								pos.y = selection_pos.y + 2 * screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd - 1][xd - 2] < 0)
							{
								pos.x = selection_pos.x - 2 * screenWidth / 8;
								pos.y = selection_pos.y - screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd - 1][xd + 2] < 0)
							{
								pos.x = selection_pos.x + 2 * screenWidth / 8;
								pos.y = selection_pos.y - screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd + 1][xd - 2] < 0)
							{
								pos.x = selection_pos.x - 2 * screenWidth / 8;
								pos.y = selection_pos.y + screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd + 1][xd + 2] < 0)
							{
								pos.x = selection_pos.x + 2 * screenWidth / 8;
								pos.y = selection_pos.y + screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
						}

						if (piece == -pieces.Knight)
						{
							Vector2 pos = selection_pos;

							if (board[yd - 2][xd - 1] > 0)
							{
								pos.x = selection_pos.x - screenWidth / 8;
								pos.y = selection_pos.y - 2 * screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd - 2][xd + 1] > 0)
							{
								pos.x = selection_pos.x + screenWidth / 8;
								pos.y = selection_pos.y - 2 * screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd + 2][xd - 1] > 0)
							{
								pos.x = selection_pos.x - screenWidth / 8;
								pos.y = selection_pos.y + 2 * screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd + 2][xd + 1] > 0)
							{
								pos.x = selection_pos.x + screenWidth / 8;
								pos.y = selection_pos.y + 2 * screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd - 1][xd - 2] > 0)
							{
								pos.x = selection_pos.x - 2 * screenWidth / 8;
								pos.y = selection_pos.y - screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd - 1][xd + 2] > 0)
							{
								pos.x = selection_pos.x + 2 * screenWidth / 8;
								pos.y = selection_pos.y - screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd + 1][xd - 2] > 0)
							{
								pos.x = selection_pos.x - 2 * screenWidth / 8;
								pos.y = selection_pos.y + screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
							if (board[yd + 1][xd + 2] > 0)
							{
								pos.x = selection_pos.x + 2 * screenWidth / 8;
								pos.y = selection_pos.y + screenWidth / 8;
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
							}
						}
						DrawTextureRec(selection, Tselection.rec, step, WHITE);
					}

					step.x = step.x + screenWidth / 8;
				}
				step.y = step.y + screenWidth / 8;
				step.x = selection_pos.x - 2 * screenWidth / 8;
			}
		}
		//------------------------------------------------------------------------------------------------------
		// bishop:

		if ((piece == pieces.bishop) || (piece == -pieces.bishop))
		{

			DrawTextureRec(SelectionPiece, tSelectionPiece.rec, selection_pos, WHITE);

			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				for (int i = 0; i < 8; i++)
				{
					if (i == j)
					{
						DrawTextureRec(selection, Tselection.rec, step, WHITE);
						if ((board[yd - j][xd - i] != 0) && (xd - i != xd) && (yd - j != yd))
						{
							if (piece == pieces.bishop && board[yd - j][xd - i] < 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							if (piece == -pieces.bishop && board[yd - j][xd - i] > 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							j = 8;
						}
					}
					step.x = step.x - screenWidth / 8;
				}
				step.y = step.y - screenWidth / 8;
				step.x = selection_pos.x;
			}
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				for (int i = 0; i < 8; i++)
				{
					if (i == j)
					{
						DrawTextureRec(selection, Tselection.rec, step, WHITE);
						if ((board[yd + j][xd + i] != 0) && (xd + i != xd) && (yd + j != yd))
						{
							if (piece == pieces.bishop && board[yd + j][xd + i] < 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							if (piece == -pieces.bishop && board[yd + j][xd + i] > 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							j = 8;
						}
					}
					step.x = step.x + screenWidth / 8;
				}
				step.y = step.y + screenWidth / 8;
				step.x = selection_pos.x;
			}

			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 8; j > 0; j--)
			{
				for (int i = 0; i < 8; i++)
				{
					if (i + j == 8)
					{
						DrawTextureRec(selection, Tselection.rec, step, WHITE);

						if ((board[yd - i][xd + i] != 0) && (xd + i != xd) && (yd - i != yd))
						{
							if (piece == pieces.bishop && board[yd - i][xd + i] < 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							if (piece == -pieces.bishop && board[yd - i][xd + i] > 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							j = 0;
						}
					}
					step.x = step.x + screenWidth / 8;
				}
				step.y = step.y - screenWidth / 8;
				step.x = selection_pos.x;
			}
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				for (int i = 8; i > 0; i--)
				{
					if (i + j == 8)
					{
						DrawTextureRec(selection, Tselection.rec, step, WHITE);
						if ((board[yd + j][xd - j] != 0) && (xd - j != xd) && (yd + j != yd))
						{
							if (piece == pieces.bishop && board[yd + j][xd - j] < 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							if (piece == -pieces.bishop && board[yd + j][xd - j] > 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							j = 8;
						}
					}
					step.x = step.x - screenWidth / 8;
				}
				step.y = step.y + screenWidth / 8;
				step.x = selection_pos.x;
			}
		}

		//------------------------------------------------------------------------------------------------------
		// queen:
		if ((piece == pieces.queen) || (piece == -pieces.queen))
		{

			DrawTextureRec(SelectionPiece, tSelectionPiece.rec, selection_pos, WHITE);
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{

				DrawTextureRec(selection, Tselection.rec, step, WHITE);
				if (xd - j >= 0)
				{
					if ((board[yd][xd - j] != 0) && (xd - j != xd))
					{
						if (piece == pieces.queen && board[yd][xd - j] < 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						if (piece == -pieces.queen && board[yd][xd - j] > 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						j = 8;
					}
				}
				step.x = step.x - screenWidth / 8;
			}
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				DrawTextureRec(selection, Tselection.rec, step, WHITE);
				if (xd + j <= 7)
				{
					if ((board[yd][xd + j] != 0) && (xd + j != xd))
					{
						if (piece == pieces.queen && board[yd][xd + j] < 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						if (piece == -pieces.queen && board[yd][xd + j] > 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						j = 8;
					}
				}

				step.x = step.x + screenWidth / 8;
			}
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				DrawTextureRec(selection, Tselection.rec, step, WHITE);
				if (yd + j <= 7)
				{

					if ((board[yd + j][xd] != 0) && (yd + j != yd))
					{
						if (piece == pieces.queen && board[yd + j][xd] < 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						if (piece == -pieces.queen && board[yd + j][xd] > 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						j = 8;
					}
				}
				step.y = step.y + screenWidth / 8;
			}
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				DrawTextureRec(selection, Tselection.rec, step, WHITE);
				if (yd - j >= 0)
				{

					if ((board[yd - j][xd] != 0) && (yd - j != yd))
					{
						if (piece == pieces.queen && board[yd - j][xd] < 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						if (piece == -pieces.queen && board[yd - j][xd] > 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						j = 8;
					}
				}

				step.y = step.y - screenWidth / 8;
			}
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				for (int i = 0; i < 8; i++)
				{
					if (i == j)
					{
						DrawTextureRec(selection, Tselection.rec, step, WHITE);
						if ((board[yd - j][xd - i] != 0) && (xd - i != xd) && (yd - j != yd))
						{
							if (piece == pieces.queen && board[yd - j][xd - i] < 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							if (piece == -pieces.queen && board[yd - j][xd - i] > 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							j = 8;
						}
					}
					step.x = step.x - screenWidth / 8;
				}
				step.y = step.y - screenWidth / 8;
				step.x = selection_pos.x;
			}
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				for (int i = 0; i < 8; i++)
				{
					if (i == j)
					{
						DrawTextureRec(selection, Tselection.rec, step, WHITE);
						if ((board[yd + j][xd + i] != 0) && (xd + i != xd) && (yd + j != yd))
						{
							if (piece == pieces.queen && board[yd + j][xd + i] < 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							if (piece == -pieces.queen && board[yd + j][xd + i] > 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							j = 8;
						}
					}
					step.x = step.x + screenWidth / 8;
				}
				step.y = step.y + screenWidth / 8;
				step.x = selection_pos.x;
			}

			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 8; j > 0; j--)
			{
				for (int i = 0; i < 8; i++)
				{
					if (i + j == 8)
					{
						DrawTextureRec(selection, Tselection.rec, step, WHITE);

						if ((board[yd - i][xd + i] != 0) && (xd + i != xd) && (yd - i != yd))
						{
							if (piece == pieces.queen && board[yd - i][xd + i] < 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							if (piece == -pieces.queen && board[yd - i][xd + i] > 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							j = 0;
						}
					}
					step.x = step.x + screenWidth / 8;
				}
				step.y = step.y - screenWidth / 8;
				step.x = selection_pos.x;
			}
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				for (int i = 8; i > 0; i--)
				{
					if (i + j == 8)
					{
						DrawTextureRec(selection, Tselection.rec, step, WHITE);
						if ((board[yd + j][xd - j] != 0) && (xd - j != xd) && (yd + j != yd))
						{
							if (piece == pieces.queen && board[yd + j][xd - j] < 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							if (piece == -pieces.queen && board[yd + j][xd - j] > 0)
							{
								DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
							}
							j = 8;
						}
					}
					step.x = step.x - screenWidth / 8;
				}
				step.y = step.y + screenWidth / 8;
				step.x = selection_pos.x;
			}
		}

		//----------------------------------------------------------------
		// rook:

		if ((piece == pieces.rook) || (piece == -pieces.rook))
		{

			DrawTextureRec(SelectionPiece, tSelectionPiece.rec, selection_pos, WHITE);
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{

				DrawTextureRec(selection, Tselection.rec, step, WHITE);
				if (xd - j >= 0)
				{
					if ((board[yd][xd - j] != 0) && (xd - j != xd))
					{
						if (piece == pieces.rook && board[yd][xd - j] < 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						if (piece == -pieces.rook && board[yd][xd - j] > 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						j = 8;
					}
				}
				step.x = step.x - screenWidth / 8;
			}
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				DrawTextureRec(selection, Tselection.rec, step, WHITE);
				if (xd + j <= 7)
				{
					if ((board[yd][xd + j] != 0) && (xd + j != xd))
					{
						if (piece == pieces.rook && board[yd][xd + j] < 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						if (piece == -pieces.rook && board[yd][xd + j] > 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						j = 8;
					}
				}

				step.x = step.x + screenWidth / 8;
			}
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				DrawTextureRec(selection, Tselection.rec, step, WHITE);
				if (yd + j <= 7)
				{

					if ((board[yd + j][xd] != 0) && (yd + j != yd))
					{
						if (piece == pieces.rook && board[yd + j][xd] < 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						if (piece == -pieces.rook && board[yd + j][xd] > 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						j = 8;
					}
				}
				step.y = step.y + screenWidth / 8;
			}
			step.x = selection_pos.x;
			step.y = selection_pos.y;

			for (int j = 0; j < 8; j++)
			{
				DrawTextureRec(selection, Tselection.rec, step, WHITE);
				if (yd - j >= 0)
				{

					if ((board[yd - j][xd] != 0) && (yd - j != yd))
					{
						if (piece == pieces.rook && board[yd - j][xd] < 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						if (piece == -pieces.rook && board[yd - j][xd] > 0)
						{
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, step, WHITE);
						}
						j = 8;
					}
				}

				step.y = step.y - screenWidth / 8;
			}
		}

		//------------------------------------------------------------------------------------------------------
		// king:

		if ((piece == pieces.king) || (piece == -pieces.king))
		{
			step.y = step.y - screenWidth / 8;
			step.x = step.x - screenWidth / 8;
			DrawTextureRec(SelectionPiece, tSelectionPiece.rec, selection_pos, WHITE);

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					DrawTextureRec(selection, Tselection.rec, step, WHITE);
					step.x = step.x + screenWidth / 8;

					if (piece == pieces.king)
					{
						Vector2 pos = selection_pos;

						if (board[yd + (i - 1)][xd + (j - 1)] < 0)
						{
							pos.x = selection_pos.x + (j - 1) * screenWidth / 8;
							pos.y = selection_pos.y + (i - 1) * screenWidth / 8;
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
						}
					}
					if (piece == -pieces.king)
					{
						Vector2 pos = selection_pos;

						if (board[yd + (i - 1)][xd + (j - 1)] > 0)
						{
							pos.x = selection_pos.x + (j - 1) * screenWidth / 8;
							pos.y = selection_pos.y + (i - 1) * screenWidth / 8;
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
						}
					}
				}
				step.y = step.y + screenWidth / 8;
				step.x = selection_pos.x - screenWidth / 8;
			}
		}
	}
	else
	{
		//------------------------------------------------------------------------------------------------------
		// king:

		if ((piece == pieces.king) || (piece == -pieces.king))
		{
			step.y = step.y - screenWidth / 8;
			step.x = step.x - screenWidth / 8;
			DrawTextureRec(SelectionPiece, tSelectionPiece.rec, selection_pos, WHITE);

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					DrawTextureRec(selection, Tselection.rec, step, WHITE);
					step.x = step.x + screenWidth / 8;

					if (piece == pieces.king)
					{
						Vector2 pos = selection_pos;

						if (board[yd + (i - 1)][xd + (j - 1)] < 0)
						{
							pos.x = selection_pos.x + (j - 1) * screenWidth / 8;
							pos.y = selection_pos.y + (i - 1) * screenWidth / 8;
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
						}
					}
					if (piece == -pieces.king)
					{
						Vector2 pos = selection_pos;

						if (board[yd + (i - 1)][xd + (j - 1)] > 0)
						{
							pos.x = selection_pos.x + (j - 1) * screenWidth / 8;
							pos.y = selection_pos.y + (i - 1) * screenWidth / 8;
							DrawTextureRec(AttackPiece, tSelectionPiece.rec, pos, WHITE);
						}
					}
				}
				step.y = step.y + screenWidth / 8;
				step.x = selection_pos.x - screenWidth / 8;
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
bool Possible(int ya, int xa, int yd, int xd, int piece)
{

	// Pawn:
	// blanc
	if ((piece == -pieces.Pawn))
	{

		int cpp;
		cpp = placeXPd(xd, xa, yd, ya);

		if (board[xd + 1][yd + 1] > 0)
		{
			if (((xa == xd + 1) && (ya == yd + 1)))
			{
				return true;
			}
		}

		if (board[xd + 1][yd - 1] > 0)
		{
			if (((xa == xd + 1) && (ya == yd - 1)))
			{
				return true;
			}
		}

		if (cpp != 0)
		{
			if (((xa == xd + 1) && (ya == yd)) || ((xa == xd + 2) && (ya == yd) && (xd == 1)))
			{
				return true;
			}
		}
	}

	// noir
	if ((piece == pieces.Pawn))
	{
		int cpu;
		cpu = placeXPu(xd, xa, yd, ya);

		if (board[xd - 1][yd + 1] < 0)
		{
			if (((xa == xd - 1) && (ya == yd + 1)))
			{
				return true;
			}
		}
		if (board[xd - 1][yd - 1] < 0)
		{
			if (((xa == xd - 1) && (ya == yd - 1)))
			{
				return true;
			}
		}
		if (cpu != 0)
		{
			if (((xa == xd - 1) && (ya == yd)) || ((xa == xd - 2) && (ya == yd) && (xd == 6)))
			{
				return true;
			}
		}
	}

	//----------------------------------------------------------
	// Chevalier:
	if ((piece == pieces.Knight) || (piece == -pieces.Knight))
	{
		if (((xa == xd - 2) && (ya == yd + 1)) || ((xa == xd - 1) && (ya == yd + 2)) || ((xa == xd + 1) && (ya == yd + 2)) || ((xa == xd + 2) && (ya == yd + 1)) ||
			((xa == xd + 2) && (ya == yd - 1)) || ((xa == xd + 1) && (ya == yd - 2)) || ((xa == xd - 1) && (ya == yd - 2)) || ((xa == xd - 2) && (ya == yd - 1)))
		{
			return true;
		}
	}
	//------------------------------------------------------------------------------------------------------
	// Fou:
	// blanc
	if ((piece == pieces.bishop))
	{
		int dd = placeXDd(xd, xa, yd, ya);
		int dg = placeXGd(xd, xa, yd, ya);
		int ud = placeXDu(xd, xa, yd, ya);
		int ug = placeXGu(xd, xa, yd, ya);

		if (dd != 0 && dg != 0 && ud != 0 && ug != 0)
		{
			if ((abs(xa - xd) == abs(ya - yd)))
			{
				return true;
			}
		}
	}
	// noir
	if ((piece == -pieces.bishop))
	{
		int dd = placeXnDd(xd, xa, yd, ya);
		int dg = placeXnGd(xd, xa, yd, ya);
		int ud = placeXnDu(xd, xa, yd, ya);
		int ug = placeXnGu(xd, xa, yd, ya);

		if (dd != 0 && dg != 0 && ud != 0 && ug != 0)
		{
			if ((abs(xa - xd) == abs(ya - yd)))
			{
				return true;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	// Reine:
	// noir
	if ((piece == pieces.queen))
	{
		int cp = placeXd(xd, xa, yd, ya);
		int cd = placeYd(xd, xa, yd, ya);
		int cu = placeXu(xd, xa, yd, ya);
		int cg = placeYg(xd, xa, yd, ya);
		// diagonal
		int dd = placeXDd(xd, xa, yd, ya);
		int dg = placeXGd(xd, xa, yd, ya);
		int ud = placeXDu(xd, xa, yd, ya);
		int ug = placeXGu(xd, xa, yd, ya);

		if (cg != 0 && cp != 0 && cd != 0 && cu != 0)
		{
			if ((xa == xd) || (ya == yd))
			{
				return true;
			}
		}
		if (dd != 0 && dg != 0 && ud != 0 && ug != 0)
		{
			if ((abs(xa - xd) == abs(ya - yd)))
			{
				return true;
			}
		}
	}
	// noir
	if ((piece == -pieces.queen))
	{
		int cp = placeXnd(xd, xa, yd, ya);
		int cd = placeYnd(xd, xa, yd, ya);
		int cu = placeXnu(xd, xa, yd, ya);
		int cg = placeYng(xd, xa, yd, ya);
		// diagonal
		int dd = placeXnDd(xd, xa, yd, ya);
		int dg = placeXnGd(xd, xa, yd, ya);
		int ud = placeXnDu(xd, xa, yd, ya);
		int ug = placeXnGu(xd, xa, yd, ya);

		if (cg != 0 && cp != 0 && cd != 0 && cu != 0)
		{
			if ((xa == xd) || (ya == yd))
			{
				return true;
			}
		}
		if (dd != 0 && dg != 0 && ud != 0 && ug != 0)
		{
			if ((abs(xa - xd) == abs(ya - yd)))
			{
				return true;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	// Roi:
	// blanc
	if ((piece == pieces.king))
	{
		int cp = placeXd(xd, xa, yd, ya);
		int cd = placeYd(xd, xa, yd, ya);
		int cu = placeXu(xd, xa, yd, ya);
		int cg = placeYg(xd, xa, yd, ya);
		int dd = placeXDd(xd, xa, yd, ya);
		int dg = placeXGd(xd, xa, yd, ya);
		int ud = placeXDu(xd, xa, yd, ya);
		int ug = placeXGu(xd, xa, yd, ya);

		if (cg != 0 && cp != 0 && cd != 0 && cu != 0 && dd != 0 && dg != 0 && ud != 0 && ug != 0)
		{
			if ((abs(xa - xd) == 1 || abs(ya - yd) == 1))
			{
				if ((xa == xd || ya == yd) || (abs(xa - xd) == abs(ya - yd)))
				{
					return true;
				}
			}
		}
	}
	// noir
	if ((piece == -pieces.king))
	{
		int cp = placeXnd(xd, xa, yd, ya);
		int cd = placeYnd(xd, xa, yd, ya);
		int cu = placeXnu(xd, xa, yd, ya);
		int cg = placeYng(xd, xa, yd, ya);
		int dd = placeXnDd(xd, xa, yd, ya);
		int dg = placeXnGd(xd, xa, yd, ya);
		int ud = placeXnDu(xd, xa, yd, ya);
		int ug = placeXnGu(xd, xa, yd, ya);

		if (cg != 0 && cp != 0 && cd != 0 && cu != 0 && dd != 0 && dg != 0 && ud != 0 && ug != 0)
		{
			if ((abs(xa - xd) == 1 || abs(ya - yd) == 1))
			{
				if ((xa == xd || ya == yd) || (abs(xa - xd) == abs(ya - yd)))
				{
					return true;
				}
			}
		}
	}
	//----------------------------------------------------------------
	// Tour:
	// noir
	if ((piece == pieces.rook))
	{
		int cp, cd, cu, cg;

		cp = placeXd(xd, xa, yd, ya);
		cd = placeYd(xd, xa, yd, ya);
		cu = placeXu(xd, xa, yd, ya);
		cg = placeYg(xd, xa, yd, ya);

		if (cg != 0 && cp != 0 && cd != 0 && cu != 0)
		{
			if ((xa == xd) || (ya == yd))
			{
				return true;
			}
		}
	}
	// blanc
	if ((piece == -pieces.rook))
	{
		int cp, cd, cu, cg;

		cp = placeXnd(xd, xa, yd, ya);
		cd = placeYnd(xd, xa, yd, ya);
		cu = placeXnu(xd, xa, yd, ya);
		cg = placeYng(xd, xa, yd, ya);

		if (cg != 0 && cp != 0 && cd != 0 && cu != 0)
		{
			if ((xa == xd) || (ya == yd))
			{
				return true;
			}
		}
	}

	return false;
}
//

bool check(int piece)
{
	FILE *f = fopen("save123.txt", "a");
	if (piece > 0)
	{
		bool found = false;
		int row{}, col{};
		// Finding the king on the board

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{

				if (board[i][j] == pieces.king)
				{

					row = i;
					col = j;
					found = true;
					fprintf(f, " piece white : %d ", piece);
					fprintf(f, " row : %d ", row);
					fprintf(f, " col : %d \n", col);
					fprintf(f, " found : %d \n", found);
					break;
				}
			}
		}

		if (found == false)
		{
			return false;
		}

		// Finding the king on the board
		if (row != 0 && col != 0 && board[row - 1][col - 1] == -pieces.Pawn)
			return true;
		else if (row != 0 && col != 7 && board[row - 1][col + 1] == -pieces.Pawn)
			return true;
		int a, b;
		a = row;
		b = col;
		if (a != 0 && b != 0)
		{
			for (;;)
			{
				a -= 1;
				b -= 1;
				if (board[a][b] == -pieces.queen || board[a][b] == -pieces.bishop)
					return true;
				if (board[a][b] != 0 || a == 0 || b == 0)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 0 && b != 7)
		{
			for (;;)
			{
				a -= 1;
				b += 1;
				if (board[a][b] == -pieces.queen || board[a][b] == -pieces.bishop)
					return true;
				if (board[a][b] != 0 || a == 0 || b == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 7 && b != 0)
		{
			for (;;)
			{
				a += 1;
				b -= 1;
				if (board[a][b] == -pieces.queen || board[a][b] == -pieces.bishop)
					return true;
				if (board[a][b] != 0 || a == 7 || b == 0)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 7 && b != 7)
		{
			for (;;)
			{
				a += 1;
				b += 1;
				if (board[a][b] == -pieces.queen || board[a][b] == -pieces.bishop)
					return true;
				if (board[a][b] != 0 || a == 7 || b == 7)
					break;
			}
		}

		a = row;
		b = col;
		if (a != 7)
		{
			for (;;)
			{
				a += 1;
				if (board[a][b] == -pieces.queen || board[a][b] == -pieces.rook)
					return true;
				if (board[a][b] != 0 || a == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 0)
		{
			for (;;)
			{
				a -= 1;
				if (board[a][b] == -pieces.queen || board[a][b] == -pieces.rook)
					return true;
				if (board[a][b] != 0 || a == 0)
					break;
			}
		}

		a = row;
		b = col;
		if (b != 7)
		{
			for (;;)
			{
				b += 1;
				if (board[a][b] == -pieces.queen || board[a][b] == -pieces.rook)
					return true;
				if (board[a][b] != 0 || b == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (b != 0)
		{
			for (;;)
			{
				b -= 1;
				if (board[a][b] == -pieces.queen || board[a][b] == -pieces.rook)
					return true;
				if (board[a][b] != 0 || b == 0)
					break;
			}
		}

		if (row > 0 && col < 6 && board[row - 1][col + 2] == -pieces.Knight)
			return true;
		if (row > 1 && col < 7 && board[row - 2][col + 1] == -pieces.Knight)
			return true;
		if (row < 7 && col < 6 && board[row + 1][col + 2] == -pieces.Knight)
			return true;
		if (row < 6 && col < 7 && board[row + 2][col + 1] == -pieces.Knight)
			return true;
		if (row < 6 && col > 0 && board[row + 2][col - 1] == -pieces.Knight)
			return true;
		if (row < 7 && col > 1 && board[row + 1][col - 2] == -pieces.Knight)
			return true;
		if (row > 1 && col > 0 && board[row - 2][col - 1] == -pieces.Knight)
			return true;
		if (row > 0 && col > 1 && board[row - 1][col - 2] == -3)
			return true;

		if (row != 7 && board[row + 1][col] == -pieces.king)
			return true;
		if (row != 0 && board[row - 1][col] == -pieces.king)
			return true;
		if (col != 7 && board[row][col + 1] == -pieces.king)
			return true;
		if (col != 0 && board[row][col - 1] == -pieces.king)
			return true;
		if (row != 7 && col != 7 && board[row + 1][col + 1] == -pieces.king)
			return true;
		if (row != 7 && col != 0 && board[row + 1][col - 1] == -pieces.king)
			return true;
		if (row != 0 && col != 7 && board[row - 1][col + 1] == -pieces.king)
			return true;
		if (row != 0 && col != 0 && board[row - 1][col - 1] == -pieces.king)
			return true;
	}

	else if (piece < 0)
	{
		bool found = false;
		int row{}, col{};
		// Finding the king on the board

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board[i][j] == -pieces.king)
				{
					row = i;
					col = j;
					found = true;
					fprintf(f, " piece black : %d ", piece);
					fprintf(f, " row : %d ", row);
					fprintf(f, " col : %d \n", col);
					fprintf(f, " found : %d \n", found);
					break;
				}
			}
		}
		if (found == false)
		{
			return false;
		}

		// Finding the king on the board

		if (row != 7 && col != 0 && board[row + 1][col - 1] == pieces.Pawn)
			return true;
		else if (row != 7 && col != 7 && board[row + 1][col + 1] == pieces.Pawn)
			return true;

		int a, b;
		a = row;
		b = col;
		if (a != 0 && b != 0)
		{
			for (;;)
			{
				a -= 1;
				b -= 1;
				if (board[a][b] == pieces.queen || board[a][b] == pieces.bishop)
					return true;
				if (board[a][b] != 0 || a == 0 || b == 0)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 0 && b != 7)
		{
			for (;;)
			{
				a -= 1;
				b += 1;
				if (board[a][b] == pieces.queen || board[a][b] == pieces.bishop)
					return true;
				if (board[a][b] != 0 || a == 0 || b == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 7 && b != 0)
		{
			for (;;)
			{
				a += 1;
				b -= 1;
				if (board[a][b] == pieces.queen || board[a][b] == pieces.bishop)
					return true;
				if (board[a][b] != 0 || a == 7 || b == 0)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 7 && b != 7)
		{
			for (;;)
			{
				a += 1;
				b += 1;
				if (board[a][b] == pieces.queen || board[a][b] == pieces.bishop)
					return true;
				if (board[a][b] != 0 || a == 7 || b == 7)
					break;
			}
		}

		a = row;
		b = col;
		if (a != 7)
		{
			for (;;)
			{
				a += 1;
				if (board[a][b] == pieces.queen || board[a][b] == pieces.rook)
					return true;
				if (board[a][b] != 0 || a == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 0)
		{
			for (;;)
			{
				a -= 1;
				if (board[a][b] == pieces.queen || board[a][b] == pieces.rook)
					return true;
				if (board[a][b] != 0 || a == 0)
					break;
			}
		}

		a = row;
		b = col;
		if (b != 7)
		{
			for (;;)
			{
				b += 1;
				if (board[a][b] == pieces.queen || board[a][b] == pieces.rook)
					return true;
				if (board[a][b] != 0 || b == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (b != 0)
		{
			for (;;)
			{
				b -= 1;
				if (board[a][b] == pieces.queen || board[a][b] == pieces.rook)
					return true;
				if (board[a][b] != 0 || b == 0)
					break;
			}
		}

		if (row > 0 && col < 6 && board[row - 1][col + 2] == pieces.Knight)
			return true;
		if (row > 1 && col < 7 && board[row - 2][col + 1] == pieces.Knight)
			return true;
		if (row < 7 && col < 6 && board[row + 1][col + 2] == pieces.Knight)
			return true;
		if (row < 6 && col < 7 && board[row + 2][col + 1] == pieces.Knight)
			return true;
		if (row < 6 && col > 0 && board[row + 2][col - 1] == pieces.Knight)
			return true;
		if (row < 7 && col > 1 && board[row + 1][col - 2] == pieces.Knight)
			return true;
		if (row > 1 && col > 0 && board[row - 2][col - 1] == pieces.Knight)
			return true;
		if (row > 0 && col > 1 && board[row - 1][col - 2] == pieces.Knight)
			return true;

		if (row != 7 && board[row + 1][col] == pieces.king)
			return true;
		if (row != 0 && board[row - 1][col] == pieces.king)
			return true;
		if (col != 7 && board[row][col + 1] == pieces.king)
			return true;
		if (col != 0 && board[row][col - 1] == pieces.king)
			return true;
		if (row != 7 && col != 7 && board[row + 1][col + 1] == pieces.king)
			return true;
		if (row != 7 && col != 0 && board[row + 1][col - 1] == pieces.king)
			return true;
		if (row != 0 && col != 7 && board[row - 1][col + 1] == pieces.king)
			return true;
		if (row != 0 && col != 0 && board[row - 1][col - 1] == pieces.king)
			return true;
	}

	fclose(f);
	return false;
}

bool checkforcheckmate(int piece)
{
	FILE *f = fopen("save123.txt", "a");
	if (piece > 0)
	{
		bool found = false;
		int row{}, col{};
		// Finding the king on the board

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{

				if (CheckForCheckmateBoard[i][j] == pieces.king)
				{

					row = i;
					col = j;
					found = true;
					fprintf(f, " piece white : %d ", piece);
					fprintf(f, " row : %d ", row);
					fprintf(f, " col : %d \n", col);
					fprintf(f, " found : %d \n", found);
					break;
				}
			}
		}

		if (found == false)
		{
			return false;
		}

		// Finding the king on the CheckForCheckmateBoard
		if (row != 0 && col != 0 && CheckForCheckmateBoard[row - 1][col - 1] == -pieces.Pawn)
			return true;
		else if (row != 0 && col != 7 && CheckForCheckmateBoard[row - 1][col + 1] == -pieces.Pawn)
			return true;
		int a, b;
		a = row;
		b = col;
		if (a != 0 && b != 0)
		{
			for (;;)
			{
				a -= 1;
				b -= 1;
				if (CheckForCheckmateBoard[a][b] == -pieces.queen || CheckForCheckmateBoard[a][b] == -pieces.bishop)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 0 || b == 0)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 0 && b != 7)
		{
			for (;;)
			{
				a -= 1;
				b += 1;
				if (CheckForCheckmateBoard[a][b] == -pieces.queen || CheckForCheckmateBoard[a][b] == -pieces.bishop)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 0 || b == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 7 && b != 0)
		{
			for (;;)
			{
				a += 1;
				b -= 1;
				if (CheckForCheckmateBoard[a][b] == -pieces.queen || CheckForCheckmateBoard[a][b] == -pieces.bishop)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 7 || b == 0)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 7 && b != 7)
		{
			for (;;)
			{
				a += 1;
				b += 1;
				if (CheckForCheckmateBoard[a][b] == -pieces.queen || CheckForCheckmateBoard[a][b] == -pieces.bishop)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 7 || b == 7)
					break;
			}
		}

		a = row;
		b = col;
		if (a != 7)
		{
			for (;;)
			{
				a += 1;
				if (CheckForCheckmateBoard[a][b] == -pieces.queen || CheckForCheckmateBoard[a][b] == -pieces.rook)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 0)
		{
			for (;;)
			{
				a -= 1;
				if (CheckForCheckmateBoard[a][b] == -pieces.queen || CheckForCheckmateBoard[a][b] == -pieces.rook)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 0)
					break;
			}
		}

		a = row;
		b = col;
		if (b != 7)
		{
			for (;;)
			{
				b += 1;
				if (CheckForCheckmateBoard[a][b] == -pieces.queen || CheckForCheckmateBoard[a][b] == -pieces.rook)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || b == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (b != 0)
		{
			for (;;)
			{
				b -= 1;
				if (CheckForCheckmateBoard[a][b] == -pieces.queen || CheckForCheckmateBoard[a][b] == -pieces.rook)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || b == 0)
					break;
			}
		}

		if (row > 0 && col < 6 && CheckForCheckmateBoard[row - 1][col + 2] == -pieces.Knight)
			return true;
		if (row > 1 && col < 7 && CheckForCheckmateBoard[row - 2][col + 1] == -pieces.Knight)
			return true;
		if (row < 7 && col < 6 && CheckForCheckmateBoard[row + 1][col + 2] == -pieces.Knight)
			return true;
		if (row < 6 && col < 7 && CheckForCheckmateBoard[row + 2][col + 1] == -pieces.Knight)
			return true;
		if (row < 6 && col > 0 && CheckForCheckmateBoard[row + 2][col - 1] == -pieces.Knight)
			return true;
		if (row < 7 && col > 1 && CheckForCheckmateBoard[row + 1][col - 2] == -pieces.Knight)
			return true;
		if (row > 1 && col > 0 && CheckForCheckmateBoard[row - 2][col - 1] == -pieces.Knight)
			return true;
		if (row > 0 && col > 1 && CheckForCheckmateBoard[row - 1][col - 2] == -3)
			return true;

		if (row != 7 && CheckForCheckmateBoard[row + 1][col] == -pieces.king)
			return true;
		if (row != 0 && CheckForCheckmateBoard[row - 1][col] == -pieces.king)
			return true;
		if (col != 7 && CheckForCheckmateBoard[row][col + 1] == -pieces.king)
			return true;
		if (col != 0 && CheckForCheckmateBoard[row][col - 1] == -pieces.king)
			return true;
		if (row != 7 && col != 7 && CheckForCheckmateBoard[row + 1][col + 1] == -pieces.king)
			return true;
		if (row != 7 && col != 0 && CheckForCheckmateBoard[row + 1][col - 1] == -pieces.king)
			return true;
		if (row != 0 && col != 7 && CheckForCheckmateBoard[row - 1][col + 1] == -pieces.king)
			return true;
		if (row != 0 && col != 0 && CheckForCheckmateBoard[row - 1][col - 1] == -pieces.king)
			return true;
	}

	else if (piece < 0)
	{
		bool found = false;
		int row{}, col{};
		// Finding the king on the CheckForCheckmateBoard

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (CheckForCheckmateBoard[i][j] == -pieces.king)
				{
					row = i;
					col = j;
					found = true;
					fprintf(f, " piece black : %d ", piece);
					fprintf(f, " row : %d ", row);
					fprintf(f, " col : %d \n", col);
					fprintf(f, " found : %d \n", found);
					break;
				}
			}
		}
		if (found == false)
		{
			return false;
		}

		// Finding the king on the CheckForCheckmateBoard

		if (row != 7 && col != 0 && CheckForCheckmateBoard[row + 1][col - 1] == pieces.Pawn)
			return true;
		else if (row != 7 && col != 7 && CheckForCheckmateBoard[row + 1][col + 1] == pieces.Pawn)
			return true;

		int a, b;
		a = row;
		b = col;
		if (a != 0 && b != 0)
		{
			for (;;)
			{
				a -= 1;
				b -= 1;
				if (CheckForCheckmateBoard[a][b] == pieces.queen || CheckForCheckmateBoard[a][b] == pieces.bishop)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 0 || b == 0)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 0 && b != 7)
		{
			for (;;)
			{
				a -= 1;
				b += 1;
				if (CheckForCheckmateBoard[a][b] == pieces.queen || CheckForCheckmateBoard[a][b] == pieces.bishop)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 0 || b == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 7 && b != 0)
		{
			for (;;)
			{
				a += 1;
				b -= 1;
				if (CheckForCheckmateBoard[a][b] == pieces.queen || CheckForCheckmateBoard[a][b] == pieces.bishop)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 7 || b == 0)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 7 && b != 7)
		{
			for (;;)
			{
				a += 1;
				b += 1;
				if (CheckForCheckmateBoard[a][b] == pieces.queen || CheckForCheckmateBoard[a][b] == pieces.bishop)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 7 || b == 7)
					break;
			}
		}

		a = row;
		b = col;
		if (a != 7)
		{
			for (;;)
			{
				a += 1;
				if (CheckForCheckmateBoard[a][b] == pieces.queen || CheckForCheckmateBoard[a][b] == pieces.rook)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (a != 0)
		{
			for (;;)
			{
				a -= 1;
				if (CheckForCheckmateBoard[a][b] == pieces.queen || CheckForCheckmateBoard[a][b] == pieces.rook)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || a == 0)
					break;
			}
		}

		a = row;
		b = col;
		if (b != 7)
		{
			for (;;)
			{
				b += 1;
				if (CheckForCheckmateBoard[a][b] == pieces.queen || CheckForCheckmateBoard[a][b] == pieces.rook)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || b == 7)
					break;
			}
		}
		a = row;
		b = col;
		if (b != 0)
		{
			for (;;)
			{
				b -= 1;
				if (CheckForCheckmateBoard[a][b] == pieces.queen || CheckForCheckmateBoard[a][b] == pieces.rook)
					return true;
				if (CheckForCheckmateBoard[a][b] != 0 || b == 0)
					break;
			}
		}

		if (row > 0 && col < 6 && CheckForCheckmateBoard[row - 1][col + 2] == pieces.Knight)
			return true;
		if (row > 1 && col < 7 && CheckForCheckmateBoard[row - 2][col + 1] == pieces.Knight)
			return true;
		if (row < 7 && col < 6 && CheckForCheckmateBoard[row + 1][col + 2] == pieces.Knight)
			return true;
		if (row < 6 && col < 7 && CheckForCheckmateBoard[row + 2][col + 1] == pieces.Knight)
			return true;
		if (row < 6 && col > 0 && CheckForCheckmateBoard[row + 2][col - 1] == pieces.Knight)
			return true;
		if (row < 7 && col > 1 && CheckForCheckmateBoard[row + 1][col - 2] == pieces.Knight)
			return true;
		if (row > 1 && col > 0 && CheckForCheckmateBoard[row - 2][col - 1] == pieces.Knight)
			return true;
		if (row > 0 && col > 1 && CheckForCheckmateBoard[row - 1][col - 2] == pieces.Knight)
			return true;

		if (row != 7 && CheckForCheckmateBoard[row + 1][col] == pieces.king)
			return true;
		if (row != 0 && CheckForCheckmateBoard[row - 1][col] == pieces.king)
			return true;
		if (col != 7 && CheckForCheckmateBoard[row][col + 1] == pieces.king)
			return true;
		if (col != 0 && CheckForCheckmateBoard[row][col - 1] == pieces.king)
			return true;
		if (row != 7 && col != 7 && CheckForCheckmateBoard[row + 1][col + 1] == pieces.king)
			return true;
		if (row != 7 && col != 0 && CheckForCheckmateBoard[row + 1][col - 1] == pieces.king)
			return true;
		if (row != 0 && col != 7 && CheckForCheckmateBoard[row - 1][col + 1] == pieces.king)
			return true;
		if (row != 0 && col != 0 && CheckForCheckmateBoard[row - 1][col - 1] == pieces.king)
			return true;
	}

	fclose(f);
	return false;
}
// bool checkmate(int ya, int xa, int yd, int xd, int piece)
// {
// 	if ((kingInCheck = true) && (Possible(ya, xa, yd, xd, piece) == false))
// 	{
// 	}
// }
//
void Checkmate(int y, int x)
{

	int piece = board[y][x];
	if (piece > 0)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if ((board[i][j] <= 0) || (board[i][j] == pieces.king))
					CheckForCheckmateBoard[i][j] = board[i][j];
				else
					CheckForCheckmateBoard[i][j] = 0;
			}
		}
		FILE *f = fopen("CheckForCheckmateBoard.txt", "w");
		fprintf(f, "%d\n", piece);
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				fprintf(f, " %d ", CheckForCheckmateBoard[i][j]);
			}
			fprintf(f, "\n");
		}
		int col{}, row{};
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (CheckForCheckmateBoard[i][j] == pieces.king)
				{
					col = j;
					row = i;
					break;
				}
			}
		}
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (Possible(row + i, col + j, row, col, piece) == true)
				{
					CheckForCheckmateBoard[row + i][col + j] = pieces.king;
					CheckForCheckmateBoard[row][col] = 0;
					if (checkforcheckmate(pieces.king) == false)
					{
						fprintf(f, "checkforcheckmate = false\n");
						break;
					}
					else
					{

						fprintf(f, "checkforcheckmate = true\n");
					}
				}
			}
		}

		fprintf(f, "true2\n");
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				fprintf(f, " %d ", CheckForCheckmateBoard[i][j]);
			}
			fprintf(f, "\n");
		}
		fclose(f);
	}
}

//
int tour{};
void moves(int new_Pos_X, int new_Pos_Y, int MoveTable[], Sound pieces_SFX)
{
	int switch_variable_X{}, switch_variable_Y{}, switch_variable{};
	switch_variable_X = MoveTable[0];
	switch_variable_Y = MoveTable[1];
	switch_variable = board[switch_variable_Y][switch_variable_X];

	if (((board[new_Pos_Y][new_Pos_X] < 0) && (board[switch_variable_Y][switch_variable_X] < 0)) || ((board[new_Pos_Y][new_Pos_X] > 0) && (board[switch_variable_Y][switch_variable_X] > 0)))
	{
		freindly = true;
	}
	else
	{
		freindly = false;
	}

	if (!freindly)
	{

		if (Possible(new_Pos_X, new_Pos_Y, switch_variable_X, switch_variable_Y, switch_variable) == true)
		{
			if ((tour % 2 == 0 && switch_variable > 0) || (tour % 2 != 0 && switch_variable < 0))
			{
				if ((switch_variable == pieces.Pawn) && (new_Pos_Y == 0))
				{
					menu_dame_black = true;
				}
				if ((switch_variable == -pieces.Pawn) && (new_Pos_Y == 7))
				{
					menu_dame_white = true;
				}
				if (check(switch_variable) == true)
				{
					Checkmate(switch_variable_Y, switch_variable_X);
					kingInCheck = true;
					if (switch_variable == pieces.king || switch_variable == -pieces.king)
					{
						board[new_Pos_Y][new_Pos_X] = switch_variable;
						PlaySound(pieces_SFX);
						board[switch_variable_Y][switch_variable_X] = 0;
						tour++;
						kingInCheck = false;
					}
					else
					{
						//
					}
				}
				else
				{
					board[new_Pos_Y][new_Pos_X] = switch_variable;
					PlaySound(pieces_SFX);
					board[switch_variable_Y][switch_variable_X] = 0;
					tour++;
				}
			}
		}
	}
}
////////////////////////
// save & load

void save()
{
	FILE *f = fopen("save.txt", "w");

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			fprintf(f, "%d", board[i][j]);
		}
		// fprintf(f, "\n");
	}
	fclose(f);
}
void load()
{

	int i = 0, j = 0;
	FILE *f = fopen("save.txt", "r");
	// FILE *q = fopen("save2.txt", "w");
	while (!feof(f))
	{
		int val = fgetc(f);

		if (EOF != val)
		{
			if (val == 45)
			{
				// fputc(val, q);
				val = fgetc(f);
				board[i][j] = -(val - 48);
			}
			else
			{
				board[i][j] = (val - 48);
			}
			j++;
			if (j == 8)
			{
				j = 0;
				i++;
			}
		}
	}

	fclose(f);
	// fclose(q);
}

///////////

//////////////////
int main(void)
{
	// chessAi ai;
	// load();

	int MoveTable[2] = {-10, -10};
	int MoveTablenew[2] = {-10, -10};
	Vector2 MousePos = {-100, -100};
	bool is_black = {}, is_white = {};
	int click = {};
	InitWindow(screenWidth, screenHeight, "ESISA_CHESS_PROJECT");

	InitAudioDevice();									 // Initialize audio device
	Sound pieces_SFX = LoadSound("SFX/SoundChess1.wav"); // Load  sound
	Sound background_SFX = LoadSound("SFX/CanoninDpiano.mp3");
	int soundval = 1;
	// Load texture
	Texture2D background = LoadTexture("images/board0.png");			 // load background
	Texture2D figures = LoadTexture("images/figures.png");				 // load figures
	Texture2D SelectionPiece = LoadTexture("images/SelectionPiece.png"); // load SelectionPiece
	Texture2D selection = LoadTexture("images/Selection.png");
	Texture2D AttackPiece = LoadTexture("images/AttackPiece.png");
	Texture2D dame_black = LoadTexture("images/dame_black.png");
	Texture2D dame_white = LoadTexture("images/dame_white.png");
	//////////////////////////////////////////////////////////////////////////////
	// load menu assets
	Texture2D WallPaper = LoadTexture("images/WallPaper 1.png");
	Texture2D Exit1 = LoadTexture("images/Exit 1.png");
	Texture2D Exit1big = LoadTexture("images/exitbig 1.png");
	Texture2D GameName = LoadTexture("images/GameName 1.png");
	Texture2D option1 = LoadTexture("images/option 1.png");
	Texture2D option1big = LoadTexture("images/optionbig 1.png");
	Texture2D play1 = LoadTexture("images/play 1.png");
	Texture2D play1big = LoadTexture("images/playbig 1.png");
	Texture2D menu = LoadTexture("images/menu.png");
	Texture2D menu2 = LoadTexture("images/menu2.png");
	Texture2D blur = LoadTexture("images/blur.png");
	bool has_started{}, menu_is_opened{};

	// Define button bounds on screen
	Rectangle btnBounds = {screenWidth / 2.0f - play1.width / 2.0f, 148, (float)play1.width, (float)play1.height};
	Rectangle optbtnBounds = {screenWidth / 2.0f - option1.width / 2.0f, 226, (float)option1.width, (float)option1.height};
	Rectangle exitbtnBounds = {screenWidth / 2.0f - Exit1.width / 2.0f, 308, (float)Exit1.width, (float)Exit1.height};
	Rectangle muteaudiobtnBounds = {screenWidth / 2.0f - 30.0f, 231, 60.0f, 60.0f};
	Rectangle closeMenubtnBounds = {381, 120, 40, 40};
	Rectangle rookbtnBounds = {99, 226, 50, 60};
	Rectangle bishopbtnBounds = {305, 226, 50, 60};
	Rectangle queenbtnBounds = {236, 226, 50, 60};
	Rectangle knightbtnBounds = {166, 226, 50, 60};

	Rectangle undoMenubtnBounds = {77, 234, 60.0f, 60.0f};
	Rectangle quitgameMenubtnBounds = {314, 228, 60.0f, 60.0f};
	/////////////////////////////////////////////////////////////////////////////
	AnimData Tfigures[6]{};
	AnimData TfiguresBlack[6]{};
	AnimData tSelectionPiece{};
	tSelectionPiece.rec.x = SelectionPiece.width;
	tSelectionPiece.rec.y = SelectionPiece.height;
	tSelectionPiece.rec.width = SelectionPiece.width;
	tSelectionPiece.rec.height = SelectionPiece.height;
	AnimData Tselection{};
	Tselection.rec.x = selection.width;
	Tselection.rec.y = selection.height;
	Tselection.rec.width = selection.width;
	Tselection.rec.height = selection.height;
	for (int i = 0; i <= 5; i++)
	{
		TfiguresBlack[i].rec.x = i * figures.width / 6;
		TfiguresBlack[i].rec.y = figures.height / 2;
		TfiguresBlack[i].rec.width = figures.width / 6;
		TfiguresBlack[i].rec.height = figures.height / 2;
	}
	for (int i = 0; i <= 5; i++)
	{
		Tfigures[i].rec.x = i * figures.width / 6;
		Tfigures[i].rec.y = 0.0;
		Tfigures[i].rec.width = figures.width / 6;
		Tfigures[i].rec.height = figures.height / 2;
	}
	SetTargetFPS(60);

	SetSoundVolume(background_SFX, 0.2);
	PlaySound(background_SFX);

	while (!WindowShouldClose())
	{

		MousePos = GetMousePosition();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		if (!has_started)
		{
			DrawTextureEx(WallPaper, {0, 0}, 0.0, 1.0, WHITE);
			DrawTextureEx(GameName, {154, 40}, 0.0, 1.0, WHITE);
			if (menu_is_opened)
			{
				DrawTextureEx(menu, {0, 0}, 0.0, 1.0, WHITE);
				if (CheckCollisionPointRec(MousePos, muteaudiobtnBounds))
				{

					if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
					{
						if (soundval % 2 == 0)
						{

							PauseSound(background_SFX);
							soundval++;
						}
						else
						{

							ResumeSound(background_SFX);
							soundval++;
						}
					}
				}
				if (CheckCollisionPointRec(MousePos, closeMenubtnBounds))
				{

					if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
					{
						menu_is_opened = false;
					}
				}
			}
			else
			{
				// Check button state
				if (CheckCollisionPointRec(MousePos, btnBounds))
				{
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
					{
						DrawTextureEx(play1big, {162, 145}, 0.0, 1.0, WHITE);
						has_started = true;
					}
					else
					{
						DrawTextureEx(play1big, {162, 145}, 0.0, 1.0, WHITE);
					}
				}
				else
				{
					DrawTextureEx(play1, {166, 148}, 0.0, 1.0, WHITE);
				}
				// Check button state
				if (CheckCollisionPointRec(MousePos, optbtnBounds))
				{
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
					{

						DrawTextureEx(option1big, {130, 223}, 0.0, 1.0, WHITE);
						menu_is_opened = true;
					}
					else
					{
						DrawTextureEx(option1big, {130, 223}, 0.0, 1.0, WHITE);
					}
				}
				else
				{
					DrawTextureEx(option1, {136, 226}, 0.0, 1.0, WHITE);
				}
				// Check button state
				if (CheckCollisionPointRec(MousePos, exitbtnBounds))
				{
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
					{

						DrawTextureEx(Exit1big, {175, 306}, 0.0, 1.0, WHITE);
						CloseWindow();
					}
					else
					{
						DrawTextureEx(Exit1big, {175, 306}, 0.0, 1.0, WHITE);
					}
				}
				else
				{
					DrawTextureEx(Exit1, {180, 308}, 0.0, 1.0, WHITE);
				}
			}
		}
		else
		{
			FILE *checkf = fopen("check.txt", "a");
			DrawTextureEx(background, {0, 0}, 0.0, 1.0, WHITE);

			Drow_figures(figures, TfiguresBlack, Tfigures);

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				if (!lockmvt)
				{
					if (click % 2 == 0)
					{
						int pos_x{}, pos_y{};
						pos_x = MousePos.x / (screenWidth / 8);
						pos_y = MousePos.y / (screenHeight / 8);

						MoveTable[0] = pos_x;
						MoveTable[1] = pos_y;
						if (board[MoveTable[1]][MoveTable[0]] < 0)
						{
							is_black = true;
							is_white = false;
						}
						else if (board[MoveTable[1]][MoveTable[0]] > 0)
						{
							is_white = true;
							is_black = false;
						}
					}
					else
					{
						int pos_x{}, pos_y{};

						pos_x = MousePos.x / (screenWidth / 8);
						pos_y = MousePos.y / (screenHeight / 8);
						MoveTablenew[0] = pos_x;
						MoveTablenew[1] = pos_y;

						moves(pos_x, pos_y, MoveTable, pieces_SFX);
						if (check(board[pos_x][pos_y]) == true)
						{

							fprintf(checkf, " piece : %d \n", board[pos_x][pos_y]);
							fprintf(checkf, " pos_x : %d \n", pos_x);
							fprintf(checkf, " pos_y : %d \n", pos_y);
							fprintf(checkf, " check : %d \n-------------------------\n", check(board[pos_x][pos_y]));
							FILE *fpk = fopen("save3245.txt", "w");

							for (int i = 0; i < 8; i++)
							{
								for (int j = 0; j < 8; j++)
								{
									fprintf(fpk, "%d", board[i][j]);
								}
								fprintf(fpk, "\n");
							}
							fclose(fpk);
						}
					}
				}
			}
			else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			{
				int pos_x{}, pos_y{};
				if (!lockmvt)
				{
					pos_x = MousePos.x / (screenWidth / 8);
					pos_y = MousePos.y / (screenHeight / 8);
					MoveTablenew[0] = pos_x;
					MoveTablenew[1] = pos_y;

					moves(pos_x, pos_y, MoveTable, pieces_SFX);
				}
			}
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				click++;
			}
			if ((tour % 2 == 0 && is_white) || (tour % 2 != 0 && is_black))
			{
				if (!lockmvt)
				{
					Drow_possible_figures_moves(board[MoveTable[1]][MoveTable[0]], AttackPiece, selection, Tselection, SelectionPiece, tSelectionPiece, MoveTable[0], MoveTable[1], MoveTablenew[1], MoveTablenew[0]);
				}
			}
			if (menu_dame_black)
			{
				lockmvt = true;
				DrawTextureEx(blur, {0, 0}, 0.0, 1.0, WHITE);
				DrawTextureEx(dame_black, {0, 0}, 0.0, 1.0, WHITE);
				if ((board[MoveTablenew[1]][MoveTablenew[0]] == pieces.Pawn) && (MoveTablenew[1] == 0))
				{
					if (CheckCollisionPointRec(MousePos, rookbtnBounds))
					{
						if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
						{

							board[MoveTablenew[1]][MoveTablenew[0]] = pieces.rook;
							menu_dame_black = false;
							lockmvt = false;
						}
					}
					if (CheckCollisionPointRec(MousePos, bishopbtnBounds))
					{
						if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
						{

							board[MoveTablenew[1]][MoveTablenew[0]] = pieces.bishop;
							menu_dame_black = false;
							lockmvt = false;
						}
					}
					if (CheckCollisionPointRec(MousePos, queenbtnBounds))
					{
						if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
						{

							board[MoveTablenew[1]][MoveTablenew[0]] = pieces.queen;
							menu_dame_black = false;
							lockmvt = false;
						}
					}
					if (CheckCollisionPointRec(MousePos, knightbtnBounds))
					{
						if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
						{

							board[MoveTablenew[1]][MoveTablenew[0]] = pieces.Knight;
							menu_dame_black = false;
							lockmvt = false;
						}
					}
				}
			}
			else if (menu_dame_white)
			{
				lockmvt = true;
				DrawTextureEx(blur, {0, 0}, 0.0, 1.0, WHITE);
				DrawTextureEx(dame_white, {0, 0}, 0.0, 1.0, WHITE);
				if ((board[MoveTablenew[1]][MoveTablenew[0]] == -pieces.Pawn) && (MoveTablenew[1] == 7))
				{
					if (CheckCollisionPointRec(MousePos, rookbtnBounds))
					{
						if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
						{

							board[MoveTablenew[1]][MoveTablenew[0]] = -pieces.rook;
							menu_dame_white = false;
							lockmvt = false;
						}
					}
					if (CheckCollisionPointRec(MousePos, bishopbtnBounds))
					{
						if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
						{

							board[MoveTablenew[1]][MoveTablenew[0]] = -pieces.bishop;
							menu_dame_white = false;
							lockmvt = false;
						}
					}
					if (CheckCollisionPointRec(MousePos, queenbtnBounds))
					{
						if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
						{

							board[MoveTablenew[1]][MoveTablenew[0]] = -pieces.queen;
							menu_dame_white = false;
							lockmvt = false;
						}
					}
					if (CheckCollisionPointRec(MousePos, knightbtnBounds))
					{
						if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
						{

							board[MoveTablenew[1]][MoveTablenew[0]] = -pieces.Knight;
							menu_dame_white = false;
							lockmvt = false;
						}
					}
				}
			}
			else
			{
				Drow_figures(figures, TfiguresBlack, Tfigures);
			}
			if (IsKeyPressed(KEY_R))
			{
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						board[i][j] = resetboard[i][j];
					}
				}
			}

			if (IsKeyPressed(KEY_M))
			{
				menu_is_opened = true;
				lockmvt = true;
			}
			if (menu_is_opened)
			{
				DrawTextureEx(blur, {0, 0}, 0.0, 1.0, WHITE);
				DrawTextureEx(menu2, {0, 0}, 0.0, 1.0, WHITE);
				if (CheckCollisionPointRec(MousePos, muteaudiobtnBounds))
				{

					if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
					{
						if (soundval % 2 == 0)
						{

							PauseSound(background_SFX);
							soundval++;
						}
						else
						{

							ResumeSound(background_SFX);
							soundval++;
						}
					}
				}
				if (CheckCollisionPointRec(MousePos, closeMenubtnBounds))
				{

					if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
					{
						menu_is_opened = false;
						lockmvt = false;
					}
				}
				if (CheckCollisionPointRec(MousePos, undoMenubtnBounds))
				{

					if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
					{
						lockmvt = false;
						menu_is_opened = false;
						has_started = false;
					}
				}
				if (CheckCollisionPointRec(MousePos, quitgameMenubtnBounds))
				{

					if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
					{
						save();
						CloseWindow();
					}
				}
			}
			fclose(checkf);
		}

		EndDrawing();
	}

	UnloadTexture(background); // unload background
	UnloadTexture(figures);	   // unload figures
	UnloadSound(pieces_SFX);   // Unload sound
	UnloadSound(background_SFX);
	CloseAudioDevice(); // Close audio device
	CloseWindow();

	return 0;
}
