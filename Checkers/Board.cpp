#include "Board.h"

Board::Board(int width)
{
	this->squareWidth = width;
	generateSquares();
}

void Board::generateSquares()
{
	bool isWhite = true;
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			squares[row][col] = Square(nullptr, isWhite ? BROWN : DARKBROWN);
			isWhite = !isWhite;
		}
		isWhite = !isWhite;
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
			squares[row][col].draw(Vector2Int{ row * squareWidth, col * squareWidth }, squareWidth);
			Piece* piece = squares[row][col].getPiece();
			if (piece != nullptr)
			{
				Vector2Int location = { row * squareWidth + squareWidth / 2, col * squareWidth + squareWidth / 2 };
				piece->draw(location, (float)(squareWidth * 0.4), piece->isWhite() ? WHITE : BLACK);
			}
		}
	}
}

Vector2Int Board::getSquareLocation(Square* square)
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			if (&squares[row][col] == square)
			{
				return Vector2Int{ row, col };
			}
		}
	}
	return Vector2Int{ -1, -1 };
}

Vector2Int Board::getPieceLocation(Piece* piece)
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			if (squares[row][col].getPiece() == piece)
			{
				return Vector2Int{ row, col };
			}
		}
	}
	return Vector2Int{ -1, -1 };
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
