#pragma once
#include "Board.h"
#include "Utils.h"
#include "IBot.h"
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

	void draw();
	void update();
	Board* getBoard();
	std::vector<Move> getLegalMoves(std::vector<Piece*> pieces, bool forceReturnCaptureMoves);
	void handlePlayerInput();
	void makeMove(Move* move);
	void undoMove();
	bool isWhitesTurn() const;
	bool isGameOver();
	bool didWhiteWin() const { return !whitePlayerTurn; }

private:
	Board board;
	std::stack<Move> moves;
	IBot* bot;
	int squareWidth;
	Piece* selectedPiece;
	Square* selectedSquare;
	bool isDragging;
	bool whitePlayerTurn;
	bool isPlaying;

	void movePiece(Square* from, Square* to);
	Square* getSquareUnderMouse(Vector2);
	bool isPlayerPiece(Piece*) const;
	void highlightLegalMoves();
	void unhighlightSquares();
	void generatePieces();
};

