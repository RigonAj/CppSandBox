#include "map.h"
#include <cmath>
#include <raylib.h>
#define RAD(x) ((x) * 3.14159265358979323846 / 180.0)
Map::Map(Vector2 screen): target(LoadRenderTexture(screen.x, screen.y)),offset({screen.x/2,screen.y/2})
{
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
    EndTextureMode();
}


void Map::AddPoint(Vector2 point){
    Vector2 p1 = point + offset;
    Vector2 p2 = offset + points[index % SIZE];
    SetTexture(p1,p2);
    points[index % SIZE] = point;
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
    float width = (float)target.texture.width * scale;
    float height = (float)target.texture.height * scale;
    Vector2 origin = Vector2{width, height} / 2;
    Rectangle dest = { offset.x, offset.y, width, height };

    DrawTexturePro(target.texture, source, dest, origin, angle, WHITE);
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
