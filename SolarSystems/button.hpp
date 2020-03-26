#ifndef BUTTON
#define BUTTON

#include "defs.hpp"

class Button{
public:
    Button( int x, int y );
    ~Button();
    void clearArea();
    void newButton( int id, int x, int y, int width, int height );
    int clickAt( int x, int y );
protected:
    int tblx, tbly;
    vector<unsigned char> area;
};

#endif // BUTTON
