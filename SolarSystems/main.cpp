#include "gamestate.hpp"
#include "menu.hpp"
#include "game.hpp"
#include "config.hpp"

#define FPS 30

int keyUp = SDLK_z;
int keyDown = SDLK_s;
int keyLeft = SDLK_q;
int keyRight = SDLK_d;
int keyInventory = SDLK_e;

int main(int argc, char* argv[])
{
    init("Solar Sytems");
    atexit(quit);
    Blocks::initTexture();
    Items::initTexture();

    GameContainer container = GameContainer();
    container.addState( new Menu(), "menu" );
    container.addState( new Game(), "game" );
    container.addState( new Config(), "config" );
    container.setFps(FPS);
    cout << "Lancement du jeu" << endl;
    container.start();

    Block::destroy();
    Items::destroy();

    exit(0);

    return 0;
}
