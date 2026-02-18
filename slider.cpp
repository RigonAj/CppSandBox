#include "slider.h"
#include <algorithm>
#include <raylib.h>

slider::slider(Vector2 pose , Vector2 size, Vector2 range):
pose_(pose), size_(size),range_(range),value_(range.x){
    if(size.x < size.y){
        SlideSize_ = {size_.x,size_.y/10};
        SlidePose_ = {pose_.x,pose_.y + size_.y - SlideSize_.y};
    }
    else{
        SlideSize_ = {size_.x/10,size_.y};
        SlidePose_ = {pose_.x,pose_.y};
    }
    recSlider_ = {pose_.x,pose_.y, size_.x,size_.y};
}
void slider::Draw(){
    DrawRectangleV(pose_, size_, GRAY);
    DrawRectangleV(SlidePose_, SlideSize_, BLUE);
}

void slider::Update(){
    Slide();
}

void slider::Slide() {

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), recSlider_)) {
        move_ = true;
        offset = GetMousePosition() - SlidePose_ ;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) move_ = false;

    if (move_) {
        float percent = 0;
        if (size_.x < size_.y) {
            float target = (float)GetMouseY() - offset.y;
            SlidePose_.y = std::clamp(target, pose_.y, pose_.y + size_.y - SlideSize_.y);
            percent = 1.0f - ((SlidePose_.y - pose_.y) / (size_.y - SlideSize_.y));
        }
        else {
            float target = (float)GetMouseX() - offset.x;
            SlidePose_.x = std::clamp(target, pose_.x, pose_.x + size_.x - SlideSize_.x);
            percent = (SlidePose_.x - pose_.x) / (size_.x - SlideSize_.x);
        }
        value_ = range_.x + (percent * (range_.y - range_.x));
    }
}
