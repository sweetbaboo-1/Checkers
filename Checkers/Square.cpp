#include "Square.h"
Square::Square()
{
	piece = nullptr;
	color = RED; // dummy color
	highlighted = false;
}

Square::Square(Piece* piece, Color color)
{
	this->piece = piece;
	this->color = color;
	highlighted = false;
}

void Square::draw(IVector2 position, int width) const
{
	DrawRectangle(position.x, position.y, width, width, highlighted ? SKYBLUE : color);
}

bool Square::isHighlighted() const
{
	return highlighted;
}

void Square::setHighlight(bool highlighted)
{
	this->highlighted = highlighted;
}

void Square::setPiece(Piece* piece)
{
	this->piece = piece;
}

Piece* Square::getPiece()
{
	return piece;
}
