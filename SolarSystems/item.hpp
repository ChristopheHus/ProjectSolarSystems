#ifndef ITEM
#define ITEM

#define STACKMAX 99

#include "world.hpp"

class Entity;
class Perso;
class Inventory;
class ItemStack;

class Item{
public :
    Item( int id );
    ~Item();
    int getID() { return ID; }
    char virtual onRightClick( World *world, int x, int y, Entity *entity, Perso *perso );
    char virtual onLeftClick( World *world, int x, int y, Entity *entity, Perso *perso );
    static void destroy();
    static Item *getItemFromId( int id );
protected :
    static Item *items[512];
    int ID;

friend class Items;
};

class ItemBlock : public Item{
public:
    ItemBlock( int id );
    ~ItemBlock();
    char virtual onRightClick( World *world, int x, int y, Entity *entity, Perso *perso );
    char virtual onLeftClick( World *world, int x, int y, Entity *entity, Perso *perso );
};

class ItemDematerializer : public Item{
public:
    ItemDematerializer( int id );//int durability
    ~ItemDematerializer();
    char virtual onRightClick( World *world, int x, int y, Entity *entity, Perso *perso );
    char virtual onLeftClick( World *world, int x, int y, Entity *entity, Perso *perso );
protected:
    int cursor[2];
    unsigned char state;
};

class Items{
public:
	static ItemDematerializer *dematerializer;

    static void render( int id, int x, int y );
    static void render( int id, int x, int y, int w, int h );
    static void destroy();
    static void initTexture();
protected:
    static MultiImage *textureItems;
};

class ItemStack{
public:
    ItemStack( int itemId, int quantity );
    ~ItemStack();
    void render( int x, int y );
    int getId() { return itemID; }
    int getQuantity() { return nItems; }
    int addItems( int quantity );
    bool onRightClick( World *world, int x, int y, Entity *entity, Perso *perso );
    bool onLeftClick( World *world, int x, int y, Entity *entity, Perso *perso );
protected:
    int itemID, nItems;
};

#endif // ITEM
