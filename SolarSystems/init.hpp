#ifndef INIT
#define INIT

#include "defs.hpp"

//Fonction initialisant la SDL
void init(char*);
//Fonction quittant la SDL
void quit();
//Fonction donnant le renderer du jeu
SDL_Renderer *getrenderer();
//Fonction donnant la fenetre du jeu
SDL_Window *getscreen();
//Fonction donnant la petite police du jeu
TTF_Font *getSmallFont();
//Fonction donnant la grosse police du jeu
TTF_Font *getBigFont();
//Fonction affichant du texte a l'ecran
void drawText( string text, string color, bool fontSize, SDL_Texture *texture, int x, int y );
//Fonction transformant un entier en chaîne de caracteres
string to_string( int arg );
//Fonction permettant d'ecrire sur la console
void write( string const &text );
//Fonction fermant la console
void closeConsole();

extern const string version;
#endif // INIT
