#include "Square.h"
#include <sstream>
#include <string>
void Square::draw() {
    
    
    if (selected) 
    {
        DrawRectangle((location.x * width), (location.y * width), width, width, YELLOW);
    }
    else 
    {
        DrawRectangle((location.x * width), (location.y * width), width, width, color);
    }

    std::stringstream ss;
    ss << location.x << ", " << location.y;
    std::string str = ss.str();
    DrawText(str.c_str(),  location.x * width + width / 2 - 10,  location.y * width + width / 2 - 7, 15, this->piece == nullptr ? GREEN : BLUE);
}

void Square::setPiece(Piece* piece)
{
    this->piece = piece;
    if (piece != nullptr)
    {
        this->piece->move(IVector2{ location.x * width, location.y * width });
    }
}
