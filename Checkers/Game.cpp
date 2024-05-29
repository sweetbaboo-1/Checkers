#include "Game.h"

Game::Game(int width) : board(width), squareWidth(width), isDragging(false), selectedPiece(nullptr), selectedSquare(nullptr), whitePlayerTurn(true), gameOver(false) {}

void Game::draw()
{
    board.draw();
}

Board* Game::getBoard()
{
    return &board;
}

std::vector<Move> Game::getLegalMoves()
{
    std::vector<Move> legalMoves;
    std::vector<Move> captureMoves;

    auto isInBounds = [](IVector2 pos) -> bool {
        return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
        };

    const IVector2 whiteDirections[] = { {1, -1}, {-1, -1} };
    const IVector2 blackDirections[] = { {1, 1}, {-1, 1} };
    const IVector2 kingDirections[] = { {1, -1}, {-1, -1}, {1, 1}, {-1, 1} };

    for (auto& piece : board.getPieces())
    {
        IVector2 piecePos;
        if (piece == selectedPiece)
            piecePos = selectedSquare->getLocation();
        else
            piecePos = piece->getPositionAsVector(squareWidth);

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
                    legalMoves.push_back(Move{ piecePos, movePos, nullptr, nullptr, false });
                }
                else if (moveSquare->getPiece()->isWhite() != piece->isWhite()) 
                {
                    IVector2 capturePos = { movePos.x + directions[i].x, movePos.y + directions[i].y };
                    if (isInBounds(capturePos)) 
                    {
                        Square* captureSquare = board.getSquareAt(capturePos.x, capturePos.y);
                        if (captureSquare->getPiece() == nullptr) 
                        {
                            Move captureMove = { piecePos, capturePos, moveSquare->getPiece(), moveSquare, false };
                            captureMoves.push_back(captureMove);
                            legalMoves.push_back(captureMove);
                        }
                    }
                }
            }
        }
    }

    return captureMoves.size() > 0 ? captureMoves : legalMoves;
}

void Game::highlightLegalMoves()
{
    for (auto& move : getLegalMoves()) 
    {
        if (selectedSquare->getLocation().x == move.from.x && selectedSquare->getLocation().y == move.from.y)
        {
            board.getSquareAt(move.to.x, move.to.y)->highlight(true);
        }
    }
}

void Game::unhighlightSquares()
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            board.getSquares()[row][col].highlight(false);
        }
    }
}

void Game::movePiece(Square* from, Square* to)
{
    Piece* piece = from->getPiece();
    to->setPiece(piece);
    from->setPiece(nullptr);

    std::cerr << "Moved piece from (" << from->getLocation().x << ", " << from->getLocation().y << ") to (" << to->getLocation().x << ", " << to->getLocation().y << ")\n";
}

Square* Game::findSquareContainingPiece(Piece* piece)
{
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Square& square = board.getSquares()[row][col];
            if (square.getPiece() == piece) {
                return &square;
            }
        }
    }
    return nullptr;
}

void Game::makeMove(Move move)
{
    for (auto& legalMove : getLegalMoves())
    {
        // We need to use the legalMove here because the move that was passed is just where ever we dropped the piece
        if (move == legalMove) {
            if (legalMove.capturedPiece != nullptr)
            {
                legalMove.capturedSquare->setPiece(nullptr);
                std::cerr << "Captured piece at (" << legalMove.capturedSquare->getLocation().x << ", " << legalMove.capturedSquare->getLocation().y << ")\n";
            }
            movePiece(selectedSquare, board.getSquareAt(move.to.x, move.to.y));

            // check if we need to promote
            if (!selectedPiece->isKing() && (move.to.y == 0 || move.to.y == 7))
            {
                selectedPiece->promote();
                legalMove.promoteFlag = true;
            }

            moves.push(legalMove);
            
            // todo: if  the player who just moved captured a piece, AND the SAME piece can capture again, then it's still their turn

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

            whitePlayerTurn = !whitePlayerTurn;
            return;
        }
    }
    selectedPiece->move(IVector2{ selectedSquare->getLocation().x * selectedSquare->getWidth(), selectedSquare->getLocation().y * selectedSquare->getWidth() });
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
        board.getSquareAt(lastMove.from.x, lastMove.from.y)->getPiece()->demote();
    }
    
    // todo: handle multicapture turns
    whitePlayerTurn = !whitePlayerTurn;

    std::cerr << "Undid move from (" << lastMove.to.x << ", " << lastMove.to.y << ") to (" << lastMove.from.x << ", " << lastMove.from.y << ")\n";
}

bool Game::isWhitesTurn()
{
    return whitePlayerTurn;
}

Square* Game::getSquareUnderMouse(Vector2 mousePos)
{
    int x = (int)mousePos.x / squareWidth; 
    int y = (int)mousePos.y / squareWidth; 
    return board.getSquareAt(x, y);
}

bool Game::isPlayerPiece(Piece* piece)
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

        if (squareUnderMouse->getPiece() == nullptr)
        {
            return;
        }

        selectedSquare = squareUnderMouse;
        if (selectedSquare->getPiece() != nullptr)
        {
            selectedPiece = selectedSquare->getPiece();
            isDragging = true;
            highlightLegalMoves();
        }
    }

    if (isDragging)
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            isDragging = false;
            unhighlightSquares();
            Square* endSquare = getSquareUnderMouse(mousePos);
            makeMove(Move{ selectedSquare->getLocation(), endSquare->getLocation() });
            selectedPiece = nullptr;
            selectedSquare->highlight(false);
            selectedSquare = nullptr;
        }
        else
        {
            selectedPiece->move(IVector2{ (int)mousePos.x - squareWidth / 2, (int)mousePos.y - squareWidth / 2 });
        }
    }
}
