#pragma once
#include <time.h>
#include "IBot.h"

struct Move;
class Game;

class RandomBot : public IBot
{
public:
	Move* think(Game* game) override;
};