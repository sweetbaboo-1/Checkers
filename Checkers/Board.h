#pragma once
#include <vector>
#include "Piece.h"
#include "Square.h"
#include "Utils.h"

class Board
{
public:
	Board(int);
	~Board() = default;
	Square* getSquareAt(int row, int col);
	void draw();
	Square(*getSquares())[8] { return &squares[0]; }
	std::vector<Piece*> getPieces();

private:
	Square squares[8][8];
	void generatePieces();
	void generateSquares();
	int width;
};

