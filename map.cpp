#include "map.h"
#include <cmath>
#include <raylib.h>
#define RAD(x) ((x) * 3.14159265358979323846 / 180.0)
Map::Map(Vector2 screen): target(LoadRenderTexture(screen.x, screen.y)),offset({screen.x/2,screen.y/2})
{
    BeginTextureMode(target);
    ClearBackground(WHITE);
    EndTextureMode();
}


void Map::AddPoint(Vector2 point){
    Vector2 p1 = {offset.x + point.x,offset.y + point.y};
    Vector2 p2 = {offset.x + points[index % SIZE].x,offset.y + points[index % SIZE].y};
    SetTexture(p1,p2);
    points[index % SIZE] = point;
    index++;
}
void Map::AddPoint(float x, float y){AddPoint(Vector2{x,y});}

void Map::AddPointPolaire(Vector2 point){
   AddPoint(Vector2{point.x*sinf(RAD(point.y)),-point.x*cosf(RAD(point.y))});
}
void Map::AddPointPolaire(float distance, float angle){
   AddPoint(Vector2{distance*sinf(RAD(angle)),-distance*cosf(RAD(angle))});
}




void Map::Draw() const{
    DrawTextureRec(target.texture,
        (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 0, 0 }, WHITE);
}


void Map::SetTexture(Vector2 point1,Vector2 point2){
    BeginTextureMode(target);

    if (point2.x != 0.0f && point2.y != 0.0f ) DrawRectangleV(point2,{4,4} ,WHITE);
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
