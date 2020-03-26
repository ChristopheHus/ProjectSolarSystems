#ifndef DEFS
#define DEFS

///Fichier dans lequel sont mis toutes les importations de bibliotheque du jeu

#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <time.h>
#include <random>
#include <fstream>
#include <map>
#include <direct.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

#define SCREEN_WIDTH 768 //48*16
#define SCREEN_HEIGHT 512 //32*16

using namespace std;

extern int keyUp;
extern int keyDown;
extern int keyLeft;
extern int keyRight;
extern int keyInventory;

#endif //DEFS
