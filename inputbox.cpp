#include "inputbox.h"
#include <raylib.h>


inputboxe::inputboxe(Rectangle rec,string &s):textBox(rec),textoutput(s){buffer[MAX_CHAR + 1] ='\0';}
inputboxe::inputboxe(Vector2 pose, Vector2 size, string &s):inputboxe(pose.x,pose.y,size.x,size.y,s){}
inputboxe::inputboxe(float x,float y, float width, float heigth,string &s):inputboxe(Rectangle{x,y,width,heigth},s){}



void inputboxe::Draw(){
    DrawRectangleRec(textBox, LIGHTGRAY);
    DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGREEN);

    DrawText(buffer, (int)textBox.x + 5, (int)textBox.y + 8, 40, DARKGREEN);
}

void inputboxe::Update(){
    if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
    else mouseOnText = false;

    if (mouseOnText)
    {
        // Set the window's cursor to the I-Beam
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (letterCount < MAX_CHAR))
            {
                buffer[letterCount] = (char)key;
                buffer[letterCount+1] = '\0'; // Add null terminator at the end of the string
                letterCount++;
            }

                key = GetCharPressed();  // Check next character in the queue
        }
        if (IsKeyPressed(KEY_ENTER)) textoutput = string(buffer);

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            buffer[letterCount] = '\0';
        }
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

}

// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}
