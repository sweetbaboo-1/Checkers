#include "SmartBot.h"

Move* SmartBot::think(Game* game)
{
	this->game = game;
	this->board = game->getBoard();
	amWhite = game->isWhitesTurn();

	auto moves = game->getLegalMoves(game->getBoard()->getPieces(), false);
	Move* bestMove = nullptr;
	int bestValue = -10000;
	int alpha = -10000;
	int beta = 10000;
	int depth = 8; 
	int value;

	for (auto& move : moves)
	{
		auto copiedMove = new Move(move); 
		game->makeMove(copiedMove);
		value = minimax(depth - 1, alpha, beta, false);
		game->undoMove();
		if (value > bestValue)
		{
			bestValue = value;
			if (bestMove != nullptr)
			{
				delete bestMove; 
			}
			bestMove = copiedMove; 
		}
		else
		{
			delete copiedMove; 
		}
	}
	std::cerr << "Best move value: " << bestValue << std::endl; 
	return bestMove;
}


int SmartBot::evaluate()
{
	int whiteScore = EvaluateSide(true);
	int blackScore = EvaluateSide(false);
	return game->isWhitesTurn() ?  (whiteScore - blackScore) : (blackScore - whiteScore);
}

int SmartBot::minimax(int depth, int alpha, int beta, bool isMaximizingPlayer)
{
	if (depth == 0 || game->isGameOver())
	{
		return evaluate();
	}

	auto moves = game->getLegalMoves(game->getBoard()->getPieces(), false);

	if (isMaximizingPlayer)
	{
		int maxEval = -10000;
		for (auto& move : moves)
		{
			game->makeMove(&move);
			moveCount++;
			int eval = minimax(depth - 1, alpha, beta, false);
			game->undoMove();
			maxEval = std::max(maxEval, eval);
			alpha = std::max(alpha, eval);
			if (beta <= alpha)
			{
				break;
			}
		}
		return maxEval;
	}
	else
	{
		int minEval = 10000;
		for (auto& move : moves)
		{
			game->makeMove(&move);
			int eval = minimax(depth - 1, alpha, beta, true);
			game->undoMove();
			minEval = std::min(minEval, eval);
			beta = std::min(beta, eval);
			if (beta <= alpha)
			{
				break;
			}
		}
		return minEval;
	}
}

int SmartBot::EvaluateSide(bool isWhite)
{
	int score = 0;
	for (auto& piece : board->getPieces())
	{
		if (piece->isWhite() == isWhite)
		{
			score += piece->isKing() ? 3 : 1;
		}
	}
	return score;
}
