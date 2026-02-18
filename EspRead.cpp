#include "raylib.h"
#include <cmath>
#include <string>
#include "test.h"
#include "TextBoxes.h"
#include "serveur.h"
#include "client.h"
#include "inputbox.h"
#define RAD(x) ((x) * 3.14159265358979323846 / 180.0)

struct euler_t {
    float yaw=0;
    float pitch=0;
    float roll=0;
};

int main() {

    Vector2i screen = {1920,1080};
    InitWindow(screen.x, screen.y, "Succès ! raylib fonctionne");
    SetTargetFPS(120);


    Font mono = LoadFontEx("Font/cascadia-mono/CascadiaMono-SemiBold.otf", 64, 0, 250);
    TextBox text("",{10,55},30,10,mono);


    TextBox MouseCoord("",{10,10},20,3);

    euler_t *buf;
    float dyaw=0.0f;
    int framecount= 0;
    float yaw=0;
    Serveur server;
    Serveur server1;
    server.init("0.0.0.0",8080);
    server1.init("0.0.0.0", 8081);

    while (!WindowShouldClose()) {
        MouseCoord.ChangeText(TextFormat("X: %d, Y: %d",GetMouseX(),GetMouseY()));
        if (server.receive()){
            buf  = (euler_t*)server.GetData();
            if(framecount++ < 60) dyaw =  buf->yaw;
            float angle = buf->yaw - dyaw;
            yaw = angle + 360*(angle<0);
            yaw -= 360 * floor((yaw+180)/360);
            text.ChangeText(TextFormat("%-5s : %5.2f", "Yaw",   yaw),        0);
            text.ChangeText(TextFormat("%-5s : %5.2f", "Pitch", buf->pitch), 1);
            text.ChangeText(TextFormat("%-5s : %5.2f", "Roll",  buf->roll),  2);
            text.ChangeText(TextFormat("%-5s : %5.2f", "dyaw",  dyaw),       3);

        }

        Vector2 startPos = {960,540};
        Vector2 endpos = Vector2{300*cosf(RAD(-yaw-90))+startPos.x,300*sinf(RAD(-yaw-90))+startPos.y};


        if(IsKeyPressed(KEY_LEFT_CONTROL))dyaw =  buf->yaw;
        BeginDrawing();
            ClearBackground(RAYWHITE);
            MouseCoord.TextDraw();
            text.TextDraw();

            DrawCircleV(startPos,300,BLACK);
            DrawCircleV(startPos,280,WHITE);
            DrawLineEx(startPos, endpos , 5, RED);

        EndDrawing();

        text.MovePosPressed();

    }

    CloseWindow();
    return 0;
}
