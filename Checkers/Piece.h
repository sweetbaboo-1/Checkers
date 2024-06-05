#pragma once
#include "raylib.h"
#include "Utils.h"

class Piece
{
public:
	Piece(bool isWhite);
	~Piece() = default;

	void draw(Vector2Int position, float radius, Color color) const;
	void setKing(bool king);
	void setBeingDragged(bool isBeingDragged);
	bool isWhite() const { return white; }
	bool isKing() const { return king; }

private:
	bool king;
	bool white;
	bool isBeingDragged;
};

