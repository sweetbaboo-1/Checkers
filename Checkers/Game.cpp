#include "Game.h"


Game::Game(int squareWidth)
{
	this->squareWidth = squareWidth;
	board = Board(squareWidth);
	generatePieces();
	isPlaying = true;
	whitePlayerTurn = true;
}

void Game::draw()
{
	board.draw();
}

void Game::update()
{
	if (!gameOver)
	{
		if (whitePlayerTurn)
		{
			handleInput();
			return;
		}

		// AI that makes random legal moves
		srand(time(NULL));
		std::vector<Move> legalMoves = getLegalMoves(board.getPieces(), false);
		if (legalMoves.size() == 0)
		{
			gameOver = true;
			return;
		}
		Move move = legalMoves[rand() % legalMoves.size()];
		makeMove(move);
		return;
	}
}

Board* Game::getBoard()
{
	return &board;
}

std::vector<Move> Game::getLegalMoves(std::vector<Piece*> pieces, bool forceReturnCaptureMoves = false)
{
	std::vector<Move> legalMoves;
	std::vector<Move> captureMoves;

	auto isInBounds = [](IVector2 pos) -> bool {
		return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
		};

	const IVector2 whiteDirections[] = { {1, -1}, {-1, -1} };
	const IVector2 blackDirections[] = { {1, 1}, {-1, 1} };
	const IVector2 kingDirections[] = { {1, -1}, {-1, -1}, {1, 1}, {-1, 1} };

	for (auto& piece : pieces)
	{
		//if (piece == nullptr)
			//continue; // idk how this happens but when both sides are randomly making moves it can happen

		if (piece->isWhite() != isWhitesTurn())
			continue;

		IVector2 piecePos = board.getPieceLocaiton(piece);

		if (piecePos.x == -1 || piecePos.y == -1)
			continue;

		const IVector2* directions = piece->isKing() ? kingDirections : (piece->isWhite() ? whiteDirections : blackDirections);
		int possibleDirections = piece->isKing() ? 4 : 2;

		for (int i = 0; i < possibleDirections; ++i)
		{
			IVector2 movePos = { piecePos.x + directions[i].x, piecePos.y + directions[i].y };
			if (isInBounds(movePos))
			{
				Square* moveSquare = board.getSquareAt(movePos.x, movePos.y);
				if (moveSquare->getPiece() == nullptr)
				{
					legalMoves.push_back(Move{ piecePos, movePos, piece, nullptr, nullptr, false, false });
				}
				else if (moveSquare->getPiece()->isWhite() != piece->isWhite())
				{
					IVector2 capturePos = { movePos.x + directions[i].x, movePos.y + directions[i].y };
					if (isInBounds(capturePos))
					{
						Square* captureSquare = board.getSquareAt(capturePos.x, capturePos.y);
						if (captureSquare->getPiece() == nullptr)
						{
							Move captureMove = { piecePos, capturePos, piece, moveSquare->getPiece(), moveSquare, false, false };
							captureMoves.push_back(captureMove);
							legalMoves.push_back(captureMove);
						}
					}
				}
			}
		}
	}
	if (!forceReturnCaptureMoves)
		return captureMoves.size() > 0 ? captureMoves : legalMoves;
	return captureMoves;
}

void Game::highlightLegalMoves()
{
	for (auto& move : getLegalMoves(board.getPieces()))
	{
		IVector2 squareLocation = board.getSquareLocation(selectedSquare);
		if (squareLocation.x == -1 || squareLocation.y == -1)
			continue;
		
		if (squareLocation.x == move.from.x && squareLocation.y == move.from.y)
		{
			board.getSquareAt(move.to.x, move.to.y)->setHighlight(true);
		}
	}
}

void Game::unhighlightSquares()
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			board.getSquares()[row][col].setHighlight(false);
		}
	}
}

void Game::checkIfGameOver()
{
	bool whiteAlive = false;
	bool blackAlive = false;
	for (auto& p : board.getPieces())
	{
		if (p->isWhite())
			whiteAlive = true;
		else
			blackAlive = true;
	}

	if (!whiteAlive || !blackAlive)
	{
		gameOver = true;
	}
}

void Game::generatePieces()
{
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 3; col++) {
			if ((col + row) % 2 == 0) {
				Piece* newPiece = new Piece(false);
				board.getSquares()[row][col].setPiece(newPiece);
			}
		}
	}

	for (int row = 0; row < 8; row++) {
		for (int col = 5; col < 8; col++) {
			if ((col + row) % 2 == 0) {
				Piece* newPiece = new Piece(true);
				board.getSquares()[row][col].setPiece(newPiece);
			}
		}
	}
}

void Game::movePiece(Square* from, Square* to)
{
	Piece* piece = from->getPiece();
	to->setPiece(piece);
	from->setPiece(nullptr);
}

void Game::makeMove(Move move)
{
	for (auto& legalMove : getLegalMoves(board.getPieces()))
	{
		// We need to use the legalMove here because the move that was passed is just where ever we dropped the piece
		if (move == legalMove) {
			if (legalMove.capturedPiece != nullptr)
			{
				legalMove.capturedSquare->setPiece(nullptr);
			}
			Piece* piece = board.getSquareAt(legalMove.from.x, legalMove.from.y)->getPiece();
			movePiece(board.getSquareAt(move.from.x, move.from.y), board.getSquareAt(move.to.x, move.to.y));

			// check if we need to promote
			if (!piece->isKing() && (move.to.y == 0 || move.to.y == 7))
			{
				piece->setKing(true);
				legalMove.promoteFlag = true;
			}

			// if  the player who just moved captured a piece, AND the SAME piece can capture again, then it's still their turn
			if (legalMove.capturedPiece != nullptr) // captured something check if that piece can still move
			{
				Piece* temp = piece;
				piece = nullptr;
				auto possibleNextMoves = getLegalMoves({ board.getSquareAt(move.to.x, move.to.y)->getPiece() }, true);
				piece = temp;

				if (possibleNextMoves.size() > 0)
				{
					legalMove.multiCapture = true;
					moves.push(legalMove);
					return;
				}
			}
			moves.push(legalMove);
			checkIfGameOver();
			whitePlayerTurn = !whitePlayerTurn;
			return;
		}
	}
}

void Game::undoMove()
{
	if (moves.size() == 0)
		return;

	Move lastMove = moves.top();
	moves.pop();
	if (lastMove.capturedSquare != nullptr)
	{
		lastMove.capturedSquare->setPiece(lastMove.capturedPiece);
	}

	movePiece(board.getSquareAt(lastMove.to.x, lastMove.to.y), board.getSquareAt(lastMove.from.x, lastMove.from.y));

	if (lastMove.promoteFlag)
	{
		board.getSquareAt(lastMove.from.x, lastMove.from.y)->getPiece()->setKing(false);
	}

	if (!lastMove.multiCapture)
		whitePlayerTurn = !whitePlayerTurn;
}

bool Game::isWhitesTurn() const
{
	return whitePlayerTurn;
}

Square* Game::getSquareUnderMouse(Vector2 mousePos)
{
	int x = (int)mousePos.x / squareWidth;
	int y = (int)mousePos.y / squareWidth;
	return board.getSquareAt(x, y);
}

bool Game::isPlayerPiece(Piece* piece) const
{
	return piece->isWhite() == isWhitesTurn();
}

void Game::handleInput()
{
	Vector2 mousePos = GetMousePosition();
	bool leftMousePressedThisFrame = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		undoMove();
	}

	if (leftMousePressedThisFrame)
	{
		Square* squareUnderMouse = getSquareUnderMouse(mousePos);

		selectedSquare = squareUnderMouse;
		if (selectedSquare->getPiece() != nullptr)
		{
			if (selectedSquare->getPiece()->isWhite() != isWhitesTurn())
			{
				selectedSquare = nullptr;
				return;
			}
			selectedPiece = selectedSquare->getPiece();
			isDragging = true;
			selectedPiece->setBeingDragged(true);
			highlightLegalMoves();
		}
		else
		{
			return;
		}
	}

	if (isDragging)
	{
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			isDragging = false;
			selectedPiece->setBeingDragged(false);
			unhighlightSquares();
			Square* endSquare = getSquareUnderMouse(mousePos);

			IVector2 fromSquareLocation = board.getSquareLocation(selectedSquare);
			if (fromSquareLocation.x == -1 || fromSquareLocation.y == -1)
				return;

			IVector2 toSquareLocation = board.getSquareLocation(endSquare);
			if (toSquareLocation.x == -1 || toSquareLocation.y == -1)
				return;

			makeMove(Move{ fromSquareLocation, toSquareLocation });
			selectedPiece = nullptr;
			selectedSquare->setHighlight(false);
			selectedSquare = nullptr;
		}
		else
		{
			selectedPiece->setBeingDragged(false);
			selectedPiece->draw(IVector2{ (int)mousePos.x, (int)mousePos.y}, squareWidth * 0.4, selectedPiece->isWhite() ? WHITE : BLACK);
			selectedPiece->setBeingDragged(true);
		}
	}
}
