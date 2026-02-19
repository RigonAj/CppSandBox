#include "map.h"


#define RAD(x) ((x) * 3.14159265358979323846 / 180.0)
Map::Map(Vector2 screen): target(LoadRenderTexture(screen.x *  4 , screen.y * 4 )),offset(screen * 4 / 2.0f)
{
    BeginTextureMode(target);
    BeginMode2D(camera);
    ClearBackground(RAYWHITE);
    EndMode2D();
    EndTextureMode();


    camera.target = screen * 4 / 2;
    camera.offset = screen / 2 ;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}


void Map::AddPoint(Vector2 point){
    Vector2 p1 = point + offset ;
    Vector2 p2 = points[index % SIZE];
    SetTexture(p1,p2);
    points[index % SIZE] = p1;
    index++;
}
void Map::AddPoint(float x, float y){AddPoint(Vector2{x,y});}

void Map::AddPointPolaire(Vector2 point){
   AddPoint(point.x * Vector2{ sinf(RAD(point.y)) , -cosf(RAD(point.y)) });
}
void Map::AddPointPolaire(float distance, float angle){
   AddPoint( distance * Vector2{ sinf(RAD(angle)) , -cosf(RAD(angle)) });
}




void Map::Draw() const {

    Rectangle source = { 0, 0, (float)target.texture.width, -(float)target.texture.height };
    float width = (float)target.texture.width;
    float height = (float)target.texture.height;
    Vector2 origin = Vector2{width, height} / 2;
    Rectangle dest = { offset.x, offset.y, width, height };
    BeginMode2D(camera);
    DrawTexturePro(target.texture, source, dest, origin, 0 , WHITE);
    EndMode2D();
}

void Map::Update(){
}

bool Map::EventUpdate(){
    return moveCamera();
}

bool Map::moveCamera(){

    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        camera.offset = GetMousePosition();
        camera.target = mouseWorldPos;
        const float zoomIncrement = 0.1f;
        camera.zoom += (wheel * zoomIncrement);
        return 1;
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        camera.target -= GetMouseDelta()/camera.zoom;
        return 1;
    }
    return 0;
}
void Map::SetTexture(Vector2 point1,Vector2 point2){
    BeginTextureMode(target);
        if (point2 == Vector2{0,0} ) DrawRectangleV(point2,{4,4} ,WHITE);
        DrawRectangleV(point1,{4,4} ,RED);
    EndTextureMode();
}


void Map::reset(){
    BeginTextureMode(target);
    ClearBackground(WHITE);
    EndTextureMode();
    for(int i = 0 ; i< SIZE ; i++)points[i]=Vector2{0,0};
    index = 0;
}
