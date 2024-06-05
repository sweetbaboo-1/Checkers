#include "RandomBot.h"

Move* RandomBot::think(Game* game)
{
	srand((unsigned int) time(NULL));
	std::vector<Move> legalMoves = game->getLegalMoves(game->getBoard()->getPieces(), false);
	return legalMoves.size() == 0 ? nullptr : &legalMoves[rand() % legalMoves.size()];
}
