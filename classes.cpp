#include "classes.h"
#include <raylib.h>


TextBox::TextBox(string s,Vector2 pos,int FontSize,int spacing,Font font):
pose_(pos),font_size_(FontSize),font_(font),spacing_(spacing){
    text_.emplace_back(s);
    size_ = MeasureTextEx(font_,s.c_str(),font_size_,spacing_);
    rec_ = {pose_.x,pose_.y,size_.x,size_.y*nLines_};
}


void TextBox::TextDraw()const{
    for(int i = 0 ; i < nLines_ ; i++){
    DrawTextEx(font_,text_[i].c_str(),{ pose_.x , pose_.y  + size_.y * i}, (float)font_size_,spacing_,BLACK) ;
    }
}

void TextBox::AppendText(const string &s){
    Vector2 newSize = MeasureTextEx(font_,s.c_str(),font_size_,spacing_);
    if (newSize.x > size_.x) size_ = newSize;
    text_.emplace_back(s);
    nLines_ ++;
    rec_ = {pose_.x,pose_.y,size_.x,size_.y*nLines_};
}

void TextBox::ChangeText(const string &s,int line){
    if (line == -1 ){
        text_.clear();
        text_.emplace_back(s);
        nLines_ = 1;
    }
    else if( line + 1 > nLines_){
        while(nLines_ <= line )AppendText(".");
        text_.back() = s;
    }
    else{
        text_[line] = s;
    }
    Vector2 newSize = MeasureTextEx(font_,s.c_str(),font_size_,spacing_);
    if (newSize.x > size_.x) size_ = newSize;
    rec_ = {pose_.x,pose_.y,size_.x,size_.y*nLines_};
}


void TextBox::MovePos(){

    int xm=GetMouseX();
    int ym=GetMouseY();

    if (CheckCollisionPointRec(GetMousePosition(), rec_)){
        int x =  xm - size_.x / 2;
        int y =  ym - size_.y * nLines_ / 2;
        SetPos(x,y);
    }

}

void TextBox::MovePosPressed(){

    int xm=GetMouseX();
    int ym=GetMouseY();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
    &&  CheckCollisionPointRec(GetMousePosition(), rec_)) Moves_ = !Moves_;

    if (Moves_){
        int x =  xm - size_.x / 2;
        int y =  ym - size_.y * nLines_ / 2;
        SetPos(x,y);
    }
}

inline void TextBox::SetPos(const Vector2 &a){
    pose_ = a;
}

inline void TextBox::SetPos(int x ,int y){
    SetPos((float)x,(float)y);
}

inline void TextBox::SetPos(float x ,float y){
    SetPos(Vector2 {x,y});
}
