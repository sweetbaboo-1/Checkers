#include "raylib.h"
#include <string>
#include <sstream>

#include "Game.h"

int main(void)
{
    int squareWidth = 100;
    InitWindow(squareWidth * 8, squareWidth * 8, "Checkers!");
    //SetTargetFPS();

    Game game(squareWidth);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        game.draw();
        game.update();
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
