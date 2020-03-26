#ifndef CONFIG
#define CONFIG

#include "gamestate.hpp"
#include "button.hpp"

class Config : public GameState {
public:
    Config();
    ~Config();
protected:
    void initialize();
	void render();
	void update();
	void leave();
	int getKeyPressed();
private:
    SDL_Texture *game;
    SDL_Rect dst;
    Button buttons;
};

#endif // CONFIG
