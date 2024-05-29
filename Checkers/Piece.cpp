#include "Piece.h"

Color Piece::getColor() 
{
	return this->color;
}

IVector2 Piece::getPosition() 
{
	return this->position;
}

void Piece::draw(int offset) {
	offset = offset / 2;
	DrawCircle( this->getPosition().x + offset,  this->getPosition().y + offset, radius * 0.8, this->getColor());
	if (king)
		DrawText("K", this->getPosition().x + offset - radius / 4, this->getPosition().y - radius / 3 + offset, radius * 0.8, GOLD);
}

void Piece::move(IVector2 newPos) {
	this->position = newPos;
}


void Piece::promote() 
{
	this->king = true;
}

void Piece::demote()
{
	this->king = false;
}

IVector2 Piece::getPositionAsVector(int width)
{
	return IVector2 { position.x / width, position.y / width };
}

Piece::Piece(IVector2 position, int radius, Color color, bool isWhite) {
	this->position = position;
	this->color = color;
	this->radius = radius;
	this->king = false;
	this->white = isWhite;
}