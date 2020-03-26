#ifndef MENU
#define MENU

#include "gamestate.hpp"

class Menu : public GameState{
public:
    Menu();
    ~Menu();
protected:
    void initialize();
	void render();
	void update();
private:
    Image *backGround;
    SDL_Color txtClr, activTxtClr;
    SDL_Surface *text;
    SDL_Texture *textureText, *buffer;
    SDL_Rect dst;
    int buttonState;
};

#endif // MENU
