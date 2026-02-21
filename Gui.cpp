#include "Gui.h"
#include <vector>

vector<vector<Gui*>> Gui::element ;

int Gui::page = -1 ;
bool Gui::EventCheck(){
    for(auto it = element[page].rbegin(); it != element[page].rend(); ++it){
        if( (*it)->active && (*it)->EventCheck() ) return true;
    }
    return false;
}

void Gui::Draw(){
    for(auto it = element[page].rbegin(); it != element[page].rend(); ++it){
        if( (*it)->active ) (*it)->Draw();
    }
}

void Gui::AddPage()
{   page++;
    element.emplace_back(vector<Gui*>{});
}

void Gui::SetPage(int page_)
{
    page = page_;
}
