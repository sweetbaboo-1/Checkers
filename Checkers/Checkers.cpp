#include "raylib.h"
#include <string>
#include <sstream>

#include "Game.h"

int main(void)
{
    int squareWidth = 100;
    InitWindow(squareWidth * 8, squareWidth * 8, "Checkers!");
    SetTargetFPS(69);

    Game game(squareWidth);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!game.isGameOver())
        {
            game.draw();
            game.handleInput();
        }
        else
        {
            DrawText("You Win!", 190, 200, 20, LIGHTGRAY);
        }
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
