#include "raylib.h"
#include <cmath>
#include <string>
#include "TextBoxes.h"
#include "serveur.h"
#include "Robot.h"
#include "slider.h"
#define RAD(x) ((x) * 3.14159265358979323846 / 180.0)


int main() {

    Vector2 screen = {1920,1080};
    InitWindow(screen.x, screen.y, "Succès ! raylib fonctionne");
    SetTargetFPS(120);


    Font mono = LoadFontEx("Font/cascadia-mono/CascadiaMono-SemiBold.otf", 64, 0, 250);
    TextBox text("",{10,55},30,10,mono);
    TextBox MouseCoord("",{10,10},20,3);


    Robot robot(screen , "0.0.0.0" , 8080);




    while (!WindowShouldClose()) {

        MouseCoord.ChangeText(TextFormat("X: %d, Y: %d",GetMouseX(),GetMouseY()));



        robot.Update();
        if(buton.EventUpdate());
        else if(robot.EventUpdate());


        if(IsKeyPressed(KEY_LEFT_CONTROL)) robot.reset();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            robot.Draw();
            MouseCoord.TextDraw();
            text.TextDraw();
            DrawFPS(10, 300);

        EndDrawing();

        text.MovePosPressed();

    }

    CloseWindow();
    return 0;
}
