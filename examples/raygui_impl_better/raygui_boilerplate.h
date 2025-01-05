#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH   800
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT  450
#endif

#ifndef WINDOW_TITLE
#define WINDOW_TITLE  "SimplePanel in RayGUI"
#endif

void OnStart();
void OnUpdate();
void OnDraw();
void OnEnd();

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    OnStart();

    while (!WindowShouldClose())
    {
        OnUpdate();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            OnDraw();

        EndDrawing();
    }

    OnEnd();

    CloseWindow();
    return 0;
}
