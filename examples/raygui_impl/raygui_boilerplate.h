#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define WINDOW_WIDTH   800
#define WINDOW_HEIGHT  450
#define WINDOW_TITLE  "SimplePanel in RayGUI"

void OnStart();
void OnUpdate();
void OnDraw();
void OnEnd();

int main()
{
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
