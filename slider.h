#pragma once
#include "raylib.h"
#include "Gui.h"
#include "VectorOverload.h"

class slider:public Gui{
    public:
        slider(Vector2 pose , Vector2 size, Vector2 range = {0,1});
        bool EventCheck();
        void Draw();
        bool Slide();
        inline float GetValue() const{return value_;}

    private:
        Vector2 pose_;
        Vector2 size_;
        Rectangle recSlider_;
        Vector2 SlidePose_;
        Vector2 SlideSize_;
        bool move_ = false;
        float value_ ;
        Vector2 range_;
        bool horizontal_;
        Vector2 offset = {0,0};
};
