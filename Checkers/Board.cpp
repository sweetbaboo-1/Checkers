#include "Board.h"

void Board::generatePieces()
{
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 3; col++) {
            if ((col + row) % 2 == 0) {
                Piece* newPiece = new Piece(IVector2{ row, col }, width / 2, BLACK, false);
                squares[row][col].setPiece(newPiece);
            }
        }
    }

    for (int row = 0; row < 8; row++) {
        for (int col = 5; col < 8; col++) {
            if ((col + row) % 2 == 0) {
                Piece* newPiece = new Piece(IVector2{ row, col }, width / 2, WHITE, true);
                squares[row][col].setPiece(newPiece);
            }
        }
    }
}


void Board::generateSquares()
{
    float isWhite = -1;
    for (int row = 0; row < 8; row++) 
    {
        for (int col = 0; col < 8; col++)
        {
            squares[row][col] = Square(IVector3{ row, col, width }, nullptr, isWhite == -1 ? BROWN : DARKBROWN);
            isWhite = -isWhite;
        }
        isWhite = -isWhite; 
    }
}

Board::Board(int width) 
{
    this->width = width;
    generateSquares();
    generatePieces();
}

Square* Board::getSquareAt(int row, int col)
{
    return &squares[row][col];
}

void Board::draw() {
    for (auto& row : squares)
    {
        for (auto& square : row)
        {
            square.draw();
        }
    }

    for (auto& row : squares)
    {
        for (auto& square : row)
        {
            if (square.getPiece() != nullptr)
            {
                square.getPiece()->draw(width);
            }
        }

    }
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
