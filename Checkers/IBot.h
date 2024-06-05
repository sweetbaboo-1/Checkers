#pragma once
#include "Game.h"

class Game;
struct Move;

class IBot
{
public:
	virtual Move* think(Game* game) = 0;
};

