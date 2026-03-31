#include "raylib.h"
#include "GameManager.hpp"

// Raylib template as it is for this

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1100;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "AcadiaGameJam");

    // game manager
    GameManager* gm  = new GameManager; // will handle all the level changing logik

    Camera2D camera = { 0 };
    camera.offset = { (float)GetScreenWidth() / 2,(float)GetScreenHeight() / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    camera.target = { 0.0f,0.0f };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        gm->manageUpdate();
        // Draw
        //----------------------------------------------------------------------------------
        gm->manageDraw();
        //----------------------------------------------------------------------------------

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    delete gm;
    return 0;
}