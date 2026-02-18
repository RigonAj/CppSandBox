#include "raylib.h"
#include <cmath>
#include <string>
#include "TextBoxes.h"
#include "serveur.h"
#include "map.h"
#include "slider.h"
#define RAD(x) ((x) * 3.14159265358979323846 / 180.0)

struct point {
    float distance=0;
    float pange=0;
    float angle=0;
};
int main() {

    Vector2 screen = {1920,1080};
    InitWindow(screen.x, screen.y, "Succès ! raylib fonctionne");
    //SetTargetFPS(120);


    Font mono = LoadFontEx("Font/cascadia-mono/CascadiaMono-SemiBold.otf", 64, 0, 250);
    TextBox text("",{10,55},30,10,mono);


    TextBox MouseCoord("",{10,10},20,3);



    float dyaw=0.0f;
    int framecount= 0;
    float yaw=0;
    Serveur server;
    server.init("0.0.0.0",8080);
    point *buf = (point*)server.GetData();;
    Map map(screen );
    slider slide(Vector2{600,30},Vector2{1500,30},{2,10});

    while (!WindowShouldClose()) {

        MouseCoord.ChangeText(TextFormat("X: %d, Y: %d",GetMouseX(),GetMouseY()));

        if (server.receive() != -1){
            text.ChangeText(TextFormat("%-5s : %5.2f", "Distance", buf->distance),0);
            text.ChangeText(TextFormat("%-5s : %5.2f", "Angle", buf->angle), 1);
            for(int i = 0; i < 10 ; i++){map.AddPointPolaire(buf[i].distance/3,buf[i].angle);}
        }

        text.ChangeText(TextFormat("%-5s : %5.3f", "Slider Value", slide.GetValue() ), 2);
        slide.Update();
        map.Update();


        if(IsKeyPressed(KEY_LEFT_CONTROL)) map.reset();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            map.Draw();
            MouseCoord.TextDraw();
            text.TextDraw();
            DrawFPS(10, 300);
            slide.Draw();
        EndDrawing();

        text.MovePosPressed();

    }

    CloseWindow();
    return 0;
}
