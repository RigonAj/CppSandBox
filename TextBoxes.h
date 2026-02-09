#ifndef CLASSES_H
#define CLASSES_H
#include <iostream>
#include <raylib.h>
#include <vector>
#include <string>


using namespace std;

class TextBox{
  public:
    TextBox(string s,Vector2 pos,int FontSize,int spacing,Font font = GetFontDefault());
    void TextDraw()const;

    void AppendText(const string &s);
    void ChangeText(const string &s,int line = -1);

    void MovePos();//Just Drag the text, MouseDown
    void MovePosPressed();//Click once to get hold of the text then again to release it

    inline void SetPos(int,int);
    inline void SetPos(float,float);
    inline void SetPos(const Vector2 &);


    bool GetMoves()const{return Moves_;};

  protected:
    vector<string> text_;

    Vector2 pose_;
    Vector2 size_;
    int nLines_ = 1;

    Font font_;
    int font_size_;
    int spacing_;
    Rectangle rec_;

    bool Moves_ = 0;
    Vector2 offset = {0,0};


};

#endif
