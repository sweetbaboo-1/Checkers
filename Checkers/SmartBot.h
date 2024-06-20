#pragma once
#include "IBot.h"
#include <vector>
#include <iostream>

class SmartBot : public IBot
{
public:
	Move* think(Game* game) override;

private:
	Game* game;
	Board* board;
	bool amWhite;
	int evaluate();
	int minimax(int depth, int alpha, int beta);
	int EvaluateSide(bool isWhite);
	int moveCount = 0;
};

