#pragma once

#include <vector>
using namespace std;
class Gui{
    public:

        virtual bool EventCheck();
        virtual void Draw();
        void Update();
        void AddPage();
        void SetPage(int page_);
        bool active = true;
        static int page ;
    protected:
        static vector<vector<Gui*>> element ;

};
