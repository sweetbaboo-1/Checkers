#include "raylib.h"
#include "Game.h"
#include "RandomBot.h"
#include "SmartBot.h"

int main(void)
{
    RandomBot bot;
    SmartBot smartBot;
    int squareWidth = 100;
    Game game(squareWidth, &smartBot);

    InitWindow(squareWidth * 8, squareWidth * 8, "Checkers!");
    //SetTargetFPS(60);   

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
