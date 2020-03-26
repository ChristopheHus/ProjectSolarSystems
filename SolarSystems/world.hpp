#ifndef WORLD
#define WORLD

#include "init.hpp"
#include "block.hpp"
#include "noise.hpp"

class Gui;

class WorldGui
{
public:
    WorldGui() { gui=nullptr; x=-1; y=-1; }
    WorldGui( Gui *gui, int x, int y ) { this->gui = gui; this->x = x; this->y = y; }
    ~WorldGui() { delete gui; }
    Gui *gui;
    int x, y;
};

enum Sides:char{LEFT=0,UP=1,RIGHT=2,DOWN=3};

struct Light
{
    char value;
    char side;
    bool fromSky;
};


///Class definissant une partie(64 blocs) d'une planete
class Chunk{
public:
	Chunk();
	Chunk( ifstream &file );
	void generate( PerlinNoise *noise, int height, int offset, int num );
	Block *getBlock( int x, int y, int ground ) { return terrain[ground][x][y]; }
	void setBlock( Block *block, int x, int y, int ground ) { terrain[ground][x][y] = block; }
	int getHeight( int x );
	Light getLightAt( int const &x, int const &y ) { return lightLvl[x][y]; }
    void setLightAt( int const &x, int const &y, char value, char side, bool fromSky ) { lightLvl[x][y].value = value; lightLvl[x][y].side = side; lightLvl[x][y].fromSky = fromSky; }
	void save( ofstream &file );
protected:
	Block *terrain[2][64][256];
	Light lightLvl[64][256];
};

///Class definissant une planete (terrain + entites)
class World{
public:
	World( long seed, int height, int offset );
	World( ifstream &file );
	~World();
	void generate();
	void render( int x, int y );
	Block *getBlock( int x, int y, int ground );//ground : 0 si premier plan et 1 si arriere plan
	void setBlock( Block *block, int x, int y, int ground );
	int getHeight( int x );
	Light getLight( int const &x, int const &y );
	void setLight( int const &x, int const &y, char const &lvl, char const &side, bool const &fromSky );
	void save( ofstream &file );
	Gui *getGuiAt( int x, int y );
protected:
    int getWorldGuiAt( int x, int y );
	void populate( int num );
	void genTree( int x, int y );
	void genOre( Block *ore, int xmin, int xmax, int ymax, int n );
	void initLight();
	void updateLight( int const &x, int const &y, bool isAir );
	PerlinNoise noise;
	int height;
	int offset;
	Chunk *chunks[256];
	SDL_Rect blockShape;
	vector<WorldGui*> guis;
};


#endif // WORLD
