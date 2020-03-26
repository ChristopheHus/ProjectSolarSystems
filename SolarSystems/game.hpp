#ifndef GAME
#define GAME

#include "gamestate.hpp"
#include "perso.hpp"
#include "container.hpp"

///Classe definissant la phase du jeu
class Game : public GameState{
public:
    //Fonction creant l'objet Game
    Game();
    //Fonction detruisant tout les objets pointes
    ~Game();
protected:
    //Fonction initialisant les variables et les pointeurs
    void initialize();
    //Fonction affichant le jeu dans la fenetre
    void render();
    //Fonction modifiant les variables en fonctions des differents evenement
    void update();
    //Fonction enregistrant le jeu
    void leave();
    //Fonction permettant le choix d'un nouveau monde ou de la sauvegarde
    int saveOrNew();
    //Fonction generant le monde la premiere fois
    void genWorld();
private:
    World *world;
    Image *guiWorld;
    SDL_Rect cursor, affWorld, dst;
    SDL_Texture *w, *game;
    Perso *player;
    bool isInInv;
    Gui *gui;
};

#endif // GAME
