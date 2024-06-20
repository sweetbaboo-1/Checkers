#pragma once
#include "Board.h"
#include "Utils.h"
#include "IBot.h"
#include <iostream>
#include <stack>

class IBot;

typedef struct Move
{
	Vector2Int from;
	Vector2Int to;
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
	Game(int squareWidth, IBot* bot);
	~Game() = default;

	bool isPlaying;

	void update();
	std::vector<Move> getLegalMoves(const std::vector<Piece*>& pieces, bool forceReturnCaptureMoves);
	void handlePlayerInput();
	void makeMove(Move* move);
	void undoMove();
	void checkIfGameOver();

	Board* getBoard() { return board; }
	void draw() { board->draw(); }
	bool isGameOver() const { return gameOver; }
	bool isWhitesTurn() const { return whitePlayerTurn; }
	bool didWhiteWin() const { return !whitePlayerTurn; }

private:
	Board* board;
	IBot* bot;
	Piece* selectedPiece;
	Square* selectedSquare;
	std::stack<Move> moves;
	int squareWidth;
	bool isDragging;
	bool whitePlayerTurn;
	bool gameOver;

	void movePiece(Square* from, Square* to);
	Square* getSquareUnderMouse(Vector2);
	void highlightLegalMoves();
	void unhighlightSquares();
	void generatePieces();

	bool isPlayerPiece(Piece* piece) const { return piece->isWhite() == isWhitesTurn(); }
};

