#pragma once
#include "Board.h"
#include "Utils.h"
#include <stack>
#include <iostream>
#include <string>

typedef struct Move
{
	IVector2 from;
	IVector2 to;
	Piece* piece;
	Piece* capturedPiece;
	Square* capturedSquare;
	bool promoteFlag;
	bool multiCapture;

	bool operator==(const Move& other) const
	{
		return from.x == other.from.x &&
			from.y == other.from.y &&
			to.x == other.to.x &&
			to.y == other.to.y;
	}
} Move;

class Game
{
public:
	Game(int squareWidth);
	~Game() = default;

	void draw();
	void update();
	Board* getBoard();
	std::vector<Move> getLegalMoves(std::vector<Piece*> pieces, bool forceReturnCaptureMoves);
	void movePiece(Square* from, Square* to);
	void handleInput(); // should be in board.cpp
	void makeMove(Move move);
	void undoMove();
	bool isWhitesTurn() const;
	bool isGameOver() const { return gameOver; }
	bool didWhiteWin() const { return !whitePlayerTurn; }

private:
	Board board;
	std::stack<Move> moves;
	Square* getSquareUnderMouse(Vector2);
	int squareWidth;
	Piece* selectedPiece;
	Square* selectedSquare;
	bool isDragging;
	bool whitePlayerTurn;
	bool gameOver;
	bool isPlaying;
	bool isPlayerPiece(Piece*) const;
	void highlightLegalMoves();
	void unhighlightSquares();
	void checkIfGameOver();
	void generatePieces();
};
