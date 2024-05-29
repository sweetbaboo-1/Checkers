#pragma once
#include "Board.h"
#include "Utils.h"
#include <stack>
#include <iostream>

typedef struct Move
{
	IVector2 from;
	IVector2 to;
	Piece* capturedPiece;
	Square* capturedSquare;
	bool promoteFlag;

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
	Game(int);
	~Game() = default;

	void draw();
	Board* getBoard();
	std::vector<Move> getLegalMoves();
	void movePiece(Square*, Square*);
	Square* findSquareContainingPiece(Piece*);
	void handleInput();
	void makeMove(Move);
	void undoMove();
	bool isWhitesTurn();
	bool isGameOver() { return gameOver; }

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
	bool isPlayerPiece(Piece*);
	void highlightLegalMoves();
	void unhighlightSquares();
};
