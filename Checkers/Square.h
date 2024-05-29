#pragma once
#include "Piece.h"

class Square {
public:
    Square(IVector3 v3, Piece* piece, Color color) : location{ v3.x, v3.y }, width(static_cast<int>(v3.z)), piece(piece), color(color) { selected = false; }
    Square() : location{ 0, 0 }, width(0), piece(nullptr), color(RED) { selected = false; }
    ~Square() = default;

    bool isSelected() { return selected; }
    void highlight(bool value) { selected = value; }
    void draw();
    void setPiece(Piece* piece);
    Piece* getPiece() const { return piece; }
    int getWidth() const { return width; }
    IVector2 getLocation() const { return location; }

private:
    IVector2 location;
    int width;
    Piece* piece; 
    Color color;
    bool selected;
};
