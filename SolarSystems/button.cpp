#include "button.hpp"

Button::Button( int x, int y )
{
    tblx = x;
    tbly = y;
    for(int i=0; i<tbly*tblx; i++)
    {
        area.push_back(0);
    }
}

Button::~Button()
{
}

void Button::clearArea()
{
    for(int j=0; j<tbly; j++)
        for(int i=0; i<tblx; i++)
            area[i+j*tblx] = 0;
}

void Button::newButton( int id, int x, int y, int width, int height )
{
    for(int j=y; j<y+height; j++)
        for(int i=x; i<x+width; i++)
            area[i+j*tblx] = id;
}

int Button::clickAt( int x, int y )
{
    return area[x+y*tblx];
}
