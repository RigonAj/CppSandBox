#include "raylib.h"
#include <string>
#include "test.h"
#include "classes.h"
#include "serveur.h"
#include "client.h"
#include "inputbox.h"

int main() {
    int server = 1;
    cout << " If you want to be server 1 or client 0 : "; cin >> server;
    Vector2i screen = {1920,1080};
    InitWindow(screen.x, screen.y, "Succès ! raylib fonctionne");
    char buffer[1024];
    string output="";
    memset(buffer,0,sizeof(buffer));
    TextBox text(TextFormat(""),{15,15},20,3);
    TextBox texte(TextFormat("Moves : "),{1000,15},20,3);
    texte.AppendText("Test Brother");
    texte.AppendText("Test Brother no ----- ");


    inputboxe Input(500.0f,500.0f,200.0f,100.0f,output);
    client cliente;
    serveur serveure;
    if(server){
        serveure.serveur_init("0.0.0.0",8080);
        texte.ChangeText("Serveur",2);
    }
    else {
        cliente.client_init("127.0.0.1",8080);
        texte.ChangeText("Client",2);
    }

    while (!WindowShouldClose()) {

        text.ChangeText(TextFormat("Mouse Coord x: %f y: %f",Input.GetPose().x,Input.GetPose().y),0);
        texte.ChangeText(string(buffer),0);
        texte.ChangeText(output,1);

        if(server){
            serveure.receive(buffer);
            serveure.send(output.c_str(),output.length());
        }
        else {
            cliente.receive(buffer);
            cliente.send(output.c_str(),output.length());
        }
        Input.Update();


        BeginDrawing();
            ClearBackground(RAYWHITE);
            Input.Draw();
            text.TextDraw();
            texte.TextDraw();
        EndDrawing();


        text.MovePosPressed();

        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            texte.MovePos();
        }

    }

    CloseWindow();
    return 0;
}
