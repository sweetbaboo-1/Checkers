#pragma once
#include <vector>
#include "Piece.h"
#include "Square.h"
#include "Utils.h"

class Board
{
public:
	Board(int squareWidth);
	~Board() = default;

	void draw();
	Square* getSquareAt(int row, int col);
	Square(*getSquares())[8] { return &squares[0]; }
	Vector2Int getSquareLocation(Square* square);
	Vector2Int getPieceLocation(Piece* piece);
	std::vector<Piece*> getPieces();

private:
	Square squares[8][8];
	std::vector<Square*> highlightedSquares;
	void generateSquares();
	int squareWidth;
};

