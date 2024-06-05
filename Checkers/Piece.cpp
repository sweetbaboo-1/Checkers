#include "Piece.h"

Piece::Piece(bool isWhite)
{
	white = isWhite;
	king = false;
	isBeingDragged = false;
}

void Piece::draw(Vector2Int position, float radius, Color color) const {
    if (king)
    {
        DrawCircle(position.x, position.y, radius, GOLD);
    }
    if (isBeingDragged)
    {
        Color c = Color{ color.r, color.b, color.g, static_cast<unsigned char>(color.a / 2) };
        DrawCircle(position.x, position.y, king ? radius * 0.9 : radius, c);
    }
    else
    {
        DrawCircle(position.x, position.y, king ? radius * 0.9 : radius, color);
    }
}

void Piece::setKing(bool king)
{
	this->king = king;
}

void Piece::setBeingDragged(bool isBeingDragged)
{
	this->isBeingDragged = isBeingDragged;
}
