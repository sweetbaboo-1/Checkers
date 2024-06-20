#include "Game.h"

Game::Game(int squareWidth, IBot* bot)
{
	this->squareWidth = squareWidth;
	board = new Board(squareWidth);
	this->bot = bot;
	generatePieces();
	isPlaying = true;
	whitePlayerTurn = true;
}

void Game::update()
{
	// todo add a way to end the game
	if (!gameOver)
	{
		// todo create gui to allow player to choose bot, play as black, rematch, etc.
		whitePlayerTurn ? handlePlayerInput() : makeMove(bot->think(this));
		checkIfGameOver();
	}
	else
	{
		std::cerr << "Game Over!" << std::endl;
		std::cerr << (didWhiteWin() ? "White wins!" : "Black wins!") << std::endl;
		isPlaying = false;
	}
}

std::vector<Move> Game::getLegalMoves(const std::vector<Piece*>& pieces, bool forceReturnCaptureMoves = false)
{
	std::vector<Move> legalMoves;
	std::vector<Move> captureMoves;

	auto isInBounds = [](Vector2Int pos) -> bool
		{
			return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
		};

	// the board is a 2d array, these represent the offsets for pieces to move diagonally
	const Vector2Int whiteDirections[] = { {1, -1}, {-1, -1} };
	const Vector2Int blackDirections[] = { {1, 1}, {-1, 1} };
	const Vector2Int kingDirections[] = { {1, -1}, {-1, -1}, {1, 1}, {-1, 1} };

	for (auto& piece : pieces)
	{
		if (piece->isWhite() != isWhitesTurn())
			continue;

		Vector2Int piecePos = board->getPieceLocation(piece);

		if (piecePos.x == -1 || piecePos.y == -1)
			continue;

		const Vector2Int* directions = piece->isKing() ? kingDirections : (piece->isWhite() ? whiteDirections : blackDirections);
		int possibleDirections = piece->isKing() ? 4 : 2;

		// for every direction a piece can move, check if there is a piece in the way, if not add the move to legalMoves
		// if there is a piece in the way, check if it can be captured, if so add the move to captureMoves
		for (int i = 0; i < possibleDirections; ++i)
		{
			Vector2Int movePos = { piecePos.x + directions[i].x, piecePos.y + directions[i].y };
			if (isInBounds(movePos))
			{
				Square* moveSquare = board->getSquareAt(movePos.x, movePos.y);
				if (moveSquare->getPiece() == nullptr) // empty square
				{
					legalMoves.push_back(Move{ piecePos, movePos, piece, nullptr, nullptr, false, false });
				}
				else if (moveSquare->getPiece()->isWhite() != piece->isWhite()) // enemy piece
				{
					Vector2Int capturePos = { movePos.x + directions[i].x, movePos.y + directions[i].y };
					if (isInBounds(capturePos))
					{
						Square* captureSquare = board->getSquareAt(capturePos.x, capturePos.y);
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

	// useful for multi-capture moves and for bot algorithms (Quiesce)
	if (forceReturnCaptureMoves)
		return captureMoves;
	return captureMoves.size() > 0 ? captureMoves : legalMoves;
}

void Game::handlePlayerInput()
{
	Vector2 mousePos = GetMousePosition();
	bool leftMousePressedThisFrame = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		undoMove();
		return;
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
			// reset everything
			isDragging = false;
			selectedPiece->setBeingDragged(false);
			unhighlightSquares();
			Square* endSquare = getSquareUnderMouse(mousePos);

			Vector2Int fromSquareLocation = board->getSquareLocation(selectedSquare);
			if (fromSquareLocation.x == -1 || fromSquareLocation.y == -1)
				return;

			Vector2Int toSquareLocation = board->getSquareLocation(endSquare);
			if (toSquareLocation.x == -1 || toSquareLocation.y == -1)
				return;

			Move move = { fromSquareLocation, toSquareLocation, selectedPiece };
			makeMove(&move);
			selectedPiece = nullptr;
			selectedSquare->setHighlight(false);
			selectedSquare = nullptr;
		}
		else
		{
			// toggle beingDragged so we can draw the ghost piece in its square, and draw a normal piece where the mouse is
			selectedPiece->setBeingDragged(false);
			selectedPiece->draw(Vector2Int{ (int)mousePos.x, (int)mousePos.y }, (float)(squareWidth * 0.4), selectedPiece->isWhite() ? WHITE : BLACK);
			selectedPiece->setBeingDragged(true);
		}
	}
}

void Game::makeMove(Move* move)
{
	if (move == nullptr) // todo end the game with an invalid move
		return;

	// todo only check legal moves for the piece that is being moved.
	for (auto& legalMove : getLegalMoves(board->getPieces())) // need to check all legal moves because otherwise we don't know if there are capture moves
	{
		// Need to use legalMove here because the move that was passed is just where we dropped the piece
		if (*move == legalMove) {
			if (legalMove.capturedPiece != nullptr)
			{
				legalMove.capturedSquare->setPiece(nullptr);
			}
			Piece* piece = board->getSquareAt(legalMove.from.x, legalMove.from.y)->getPiece();
			movePiece(board->getSquareAt(move->from.x, move->from.y), board->getSquareAt(move->to.x, move->to.y));

			if (!piece->isKing() && (move->to.y == 0 || move->to.y == 7))
			{
				piece->setKing(true);
				legalMove.promoteFlag = true;
			}

			if (legalMove.capturedPiece != nullptr)
			{
				Piece* temp = piece;
				piece = nullptr;
				auto possibleNextMoves = getLegalMoves({ board->getSquareAt(move->to.x, move->to.y)->getPiece() }, true);
				piece = temp;

				if (possibleNextMoves.size() > 0)
				{
					legalMove.multiCapture = true;
					moves.push(std::move(legalMove));
					return;
				}
			}
			moves.push(std::move(legalMove));
			whitePlayerTurn = !whitePlayerTurn;
			return;
		}
	}
}

void Game::undoMove()
{
	if (moves.size() == 0)
		return;

	Move lastMove = std::move(moves.top());
	moves.pop();
	if (lastMove.capturedSquare != nullptr)
	{
		lastMove.capturedSquare->setPiece(lastMove.capturedPiece);
	}

	movePiece(board->getSquareAt(lastMove.to.x, lastMove.to.y), board->getSquareAt(lastMove.from.x, lastMove.from.y));

	if (lastMove.promoteFlag)
	{
		board->getSquareAt(lastMove.from.x, lastMove.from.y)->getPiece()->setKing(false);
	}

	if (!lastMove.multiCapture)
		whitePlayerTurn = !whitePlayerTurn;
}

void Game::checkIfGameOver()
{
	bool whiteAlive = false;
	bool blackAlive = false;
	for (auto& p : board->getPieces())
	{
		if (p->isWhite())
			whiteAlive = true;
		else
			blackAlive = true;
		if (whiteAlive && blackAlive)
			break;
	}

	if (!whiteAlive || !blackAlive)
	{
		gameOver = true;
	}

	gameOver = getLegalMoves(board->getPieces()).size() == 0 ? true : false;
}

void Game::movePiece(Square* from, Square* to)
{
	Piece* piece = from->getPiece();
	to->setPiece(piece);
	from->setPiece(nullptr);
}

Square* Game::getSquareUnderMouse(Vector2 mousePos)
{
	int x = (int)mousePos.x / squareWidth;
	int y = (int)mousePos.y / squareWidth;
	return board->getSquareAt(x, y);
}

void Game::highlightLegalMoves()
{
	for (auto& move : getLegalMoves(board->getPieces()))
	{
		Vector2Int squareLocation = board->getSquareLocation(selectedSquare);
		if (squareLocation.x == -1 || squareLocation.y == -1)
			continue;

		if (squareLocation.x == move.from.x && squareLocation.y == move.from.y)
		{
			board->getSquareAt(move.to.x, move.to.y)->setHighlight(true);
		}
	}
}

void Game::unhighlightSquares()
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			board->getSquares()[row][col].setHighlight(false);
		}
	}
}

void Game::generatePieces()
{
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 3; col++) {
			if ((col + row) % 2 == 0) {
				Piece* newPiece = new Piece(false);
				board->getSquares()[row][col].setPiece(newPiece);
			}
		}
	}

	for (int row = 0; row < 8; row++) {
		for (int col = 5; col < 8; col++) {
			if ((col + row) % 2 == 0) {
				Piece* newPiece = new Piece(true);
				board->getSquares()[row][col].setPiece(newPiece);
			}
		}
	}
}
