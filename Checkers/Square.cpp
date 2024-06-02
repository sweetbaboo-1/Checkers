#include "Square.h"
void Square::draw() const {
    if (selected) 
        DrawRectangle((location.x * width), (location.y * width), width, width, SKYBLUE);
    else 
        DrawRectangle((location.x * width), (location.y * width), width, width, color);
}

void Square::setPiece(Piece* piece)
{
    this->piece = piece;
    if (piece != nullptr)
    {
        this->piece->move(IVector2{ location.x * width, location.y * width });
    }
}
