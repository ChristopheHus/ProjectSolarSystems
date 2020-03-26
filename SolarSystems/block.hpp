#ifndef BLOCK
#define BLOCK

#include "texture.hpp"

class ItemBlock;

///Classe definissant les proprietes des blocks utilises
class Block{
public :
    //Fonction initialisant les proprietes du bloc
	Block( int id, bool transparency );
	//Fonction pour l'instant non utilisee
	~Block();
	//Fonction retournant l'identifiant du bloc
	int getID() { return ID; }
	//Focntion retournant vrai si le bloc est transparent
	bool isBlockTransparent() { return isTransparent; }
	//Fonction supprimant tout les blocs crees
	static void destroy();
	//Fonction retournant un objet Block suivant son identifiant
	static Block *getBlockFromId( int id );
protected :
    static Block *blocks[256];
	int ID;
	bool isTransparent;
friend class Blocks;
};

///Classe incluant tout les blocs du jeu
class Blocks{
public:
    static Block *air;
	static Block *grass;
	static Block *dirt;
	static Block *stone;
	static Block *wood;
	static Block *leaf;
	static Block *coalOre;
	static Block *lava;
	static Block *copperOre;
	static Block *ironOre;
	static Block *silverOre;
	static Block *goldOre;
	static Block *platinumOre;
	static Block *leadOre;
	static Block *diamondOre;
	static Block *graphiteOre;
	static Block *chest;
	static Block *limeStone;
	static Block *basalt;
	static Block *dirtyStone;
	static Block *pinkyStone;
	static Block *greenyStone;

    //Fonctions permettant d'afficher un bloc a un endroit precis de l'ecran avec une taille precise
	static void render( Block *block, int x, int y );
	static void render( Block *block, int x, int y, int w, int h );
	//Fonction appelant la fonction destroy de Block et fermant le fichier de la texture des blocs
	static void destroy();
	//Fonction ouvrant le fichier de la texture des blocs
	static void initTexture();
protected:
    static MultiImage *textureBlocks;
};

#endif //BLOCK
