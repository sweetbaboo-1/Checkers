#include "Board.h"

Board::Board()
{
	this->squareWidth = 0;
}

Board::Board(int width)
{
	this->squareWidth = width;
	generateSquares();
}

void Board::generateSquares()
{
	float isWhite = -1;
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			squares[row][col] = Square(nullptr, isWhite == -1 ? BROWN : DARKBROWN);
			isWhite = -isWhite;
		}
		isWhite = -isWhite;
	}
}

Square* Board::getSquareAt(int row, int col)
{
	return &squares[row][col];
}

void Board::draw() 
{

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			squares[row][col].draw(IVector2{ row * squareWidth, col * squareWidth }, squareWidth);
		}
	}

	// this loop seems redundant, but is to ensure that the pieces are drawn on top of the squares
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			Piece* piece = squares[row][col].getPiece();
			if (piece != nullptr)
			{
				IVector2 location = { row * squareWidth + squareWidth / 2, col * squareWidth + squareWidth / 2 };
				piece->draw(location, squareWidth * 0.4, piece->isWhite() ? WHITE : BLACK);
			}
		}
	}
}

IVector2 Board::getSquareLocation(Square* square)
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			if (&squares[row][col] == square)
			{
				return IVector2{ row, col };
			}
		}
	}
	return IVector2{ -1, -1 };
}

IVector2 Board::getPieceLocaiton(Piece* piece)
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			if (squares[row][col].getPiece() == piece)
			{
				return IVector2{ row, col };
			}
		}
	}
	return IVector2{ -1, -1 };
}

std::vector<Piece*> Board::getPieces()
{
	std::vector<Piece*> validPieces;
	for (auto& row : squares)
	{
		for (auto& cell : row)
		{
			if (cell.getPiece() != nullptr)
			{
				validPieces.push_back(cell.getPiece());
			}
		}
	}
	return validPieces;
}
