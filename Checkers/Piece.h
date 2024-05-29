#pragma once
#include "raylib.h"
#include "Utils.h"

class Piece
{
public:
	IVector2 getPosition();
	void move(IVector2);
	Color getColor();

	void draw(int offset);
	void promote();
	void demote();
	bool isWhite() { return white; }
	IVector2 getPositionAsVector(int);
	bool isKing() { return king; }

	Piece(IVector2, int, Color, bool);
	~Piece() = default;

private:
	IVector2 position;
	Color color;
	int radius;
	bool king;
	bool white;
};

