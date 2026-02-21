#include "raylib.h"
#include <cmath>
#include <string>
#include "TextBoxes.h"
#include "serveur.h"
#include "Robot.h"
#include "slider.h"
#define RAD(x) ((x) * 3.14159265358979323846 / 180.0)

enum{
    Menu,
    Game
};
int main() {

    Vector2 screen = {1920,1080};


    InitWindow(screen.x, screen.y, "Succès ! raylib fonctionne");
    SetTargetFPS(120);
    Font mono = LoadFontEx("Font/cascadia-mono/CascadiaMono-SemiBold.otf", 64, 0, 250);

    Gui gui;
    gui.AddPage();
    slider slide(Vector2{600,20},Vector2{1000,30},Vector2{0.1,10});
    slider slide2(Vector2{600,70},Vector2{1000,30},Vector2{0,0.1});
    TextBox text("",{10,55},30,10);
    TextBox MouseCoord("",{10,10},20,3,mono);
    gui.AddPage();
    slider slide3(Vector2{600,120},Vector2{1000,30},Vector2{0,0.1});
    gui.SetPage(0);

    Robot robot(screen , "0.0.0.0" , 8080);




    while (!WindowShouldClose()) {
        Vector2 LastPoint = robot.GetLastPoint();
        MouseCoord.ChangeText(TextFormat("X: %.0f, Y: %.0f", LastPoint.x, LastPoint.y), 0);

        MouseCoord.ChangeText(TextFormat("Lidar Time Period %.0f ms , %.0f hz",robot.TimeElapsedLidar(),1/(robot.TimeElapsedLidar()/1000+0.0001)),1);
        MouseCoord.ChangeText(TextFormat("Odometry Time Period %.0f ms , %.0f hz ",robot.TimeElapsedImu(),1/(robot.TimeElapsedImu()/1000+0.0001)),2);
        MouseCoord.ChangeText(TextFormat("Yaw : %.2f",robot.yaw_),3);
        MouseCoord.ChangeText(TextFormat("Coef : %.2f",robot.coef.load()),4);
        MouseCoord.ChangeText(TextFormat("X: %d, Y: %d", GetMouseX(), GetMouseY()), 5);


        if(gui.EventCheck()){}
        else if(robot.EventUpdate()){};

        robot.Update();

        robot.coef = slide.GetValue() + slide2.GetValue();


        if(IsKeyPressed(KEY_LEFT_CONTROL)) robot.reset();
        if(IsKeyPressed(KEY_Q)) gui.SetPage(Menu);
        if(IsKeyPressed(KEY_W)) gui.SetPage(Game);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            robot.Draw();
            DrawFPS(10, 300);
            gui.Draw();
        EndDrawing();

        text.MovePosPressed();

    }

    CloseWindow();
    return 0;
}
