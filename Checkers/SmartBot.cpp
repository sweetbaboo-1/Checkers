#include "SmartBot.h"

Move* SmartBot::think(Game* game)
{
	this->game = game;
	this->board = game->getBoard();
	auto moves = game->getLegalMoves(game->getBoard()->getPieces(), false);
	Move* bestMove = nullptr;
	int bestValue = -100000;
	int alpha = -100000;
	int beta = 10000;
	int depth = 7;
	int value;
	moveCount = 0;

	for (auto& move : moves)
	{
		auto copiedMove = std::make_unique<Move>(move);
		game->makeMove(copiedMove.get());
		value = minimax(depth - 1, alpha, beta);
		game->undoMove();
		if (value > bestValue)
		{
			bestValue = value;
			bestMove = copiedMove.release();
		}
	}
	std::cerr << "Best move value: " << bestValue << std::endl << "Evaluations: " << moveCount << std::endl;
	return bestMove;
}

int SmartBot::evaluate()
{
	moveCount++;
	int whiteScore = EvaluateSide(true);
	int blackScore = EvaluateSide(false);
	return blackScore - whiteScore;
}

int SmartBot::minimax(int depth, int alpha, int beta)
{
	auto moves = game->getLegalMoves(game->getBoard()->getPieces(), false);
	if (depth == 0 || moves.size() == 0)
		return evaluate();
	
	for (auto& move : moves)
	{
		auto m = std::make_unique<Move>(move);
		game->makeMove(m.get());
		int eval = -minimax(depth - 1, -beta, -alpha);
		game->undoMove();
		if (eval >= beta)
			return beta;
		alpha = std::max(alpha, eval);
	}
	return alpha;
}

int SmartBot::EvaluateSide(bool isWhite)
{
	int score = 0;
	for (auto& piece : board->getPieces())
	{
		if (piece->isWhite() == isWhite)
			score += piece->isKing() ? 900: 100;
	}
	return score;
}
