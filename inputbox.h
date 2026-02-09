#include <raylib.h>
#include <string>
#include <iostream>
using namespace std;
#define MAX_CHAR 9
class inputboxe{
    public:
        inputboxe(Rectangle, string &s);
        inputboxe(Vector2,Vector2, string &s);
        inputboxe(float x,float y, float width, float heigth, string &s);
        void Draw();
        void Update();
        inline Vector2 GetPose()const{ return Vector2{textBox.x,textBox.y};}
    protected:
        char buffer[MAX_CHAR];
        Rectangle textBox;
        bool mouseOnText = 0;
        int letterCount = 0 ;
        string &textoutput;
};
