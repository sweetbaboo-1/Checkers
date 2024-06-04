#pragma once
#include "Piece.h"

class Square {

public:
    Square();
    Square(Piece* piece, Color color);
    ~Square() = default;

    bool isHighlighted() const;
    void setHighlight(bool highlighted);
    void draw(IVector2 position, int width) const;
    void setPiece(Piece* piece);
    Piece* getPiece();

private:
    Piece* piece; 
    Color color;
    bool highlighted;
};
