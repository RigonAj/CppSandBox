#include "raylib.h"
#include <cmath>
#include <string>
#include "TextBoxes.h"
#include "serveur.h"
#include "inputbox.h"
#include "map.h"
#define RAD(x) ((x) * 3.14159265358979323846 / 180.0)

struct point {
    float distance=0;
    float pange=0;
    float angle=0;
};
int main() {

    Vector2 screen = {1920,1080};
    InitWindow(screen.x, screen.y, "Succès ! raylib fonctionne");
    SetTargetFPS(60);


    Font mono = LoadFontEx("Font/cascadia-mono/CascadiaMono-SemiBold.otf", 64, 0, 250);
    TextBox text("",{10,55},30,10,mono);


    TextBox MouseCoord("",{10,10},20,3);

    point *buf;
    float dyaw=0.0f;
    int framecount= 0;
    float yaw=0;
    Serveur server;
    server.init("0.0.0.0",8080);
    Map map(screen);

    while (!WindowShouldClose()) {
        MouseCoord.ChangeText(TextFormat("X: %d, Y: %d",GetMouseX(),GetMouseY()));
        if (server.receive()){
            buf  = (point*)server.GetData();
            text.ChangeText(TextFormat("%-5s : %5.2f", "Distance", buf->distance),0);
            text.ChangeText(TextFormat("%-5s : %5.2f", "Angle", buf->angle), 1);
            map.AddPointPolaire(buf->distance/3,buf->angle);
        }




        if(IsKeyPressed(KEY_LEFT_CONTROL)) map.reset();
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) map.SetScale(map.GetScale() * 1.1);
        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) map.SetScale(map.GetScale() * 0.9);
        map.angle +=12*GetMouseWheelMove();
        BeginDrawing();
            ClearBackground(RAYWHITE);
            map.Draw();
            MouseCoord.TextDraw();
            text.TextDraw();
        EndDrawing();

        text.MovePosPressed();

    }

    CloseWindow();
    return 0;
}
