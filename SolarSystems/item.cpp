#include "item.hpp"
#include "perso.hpp"
#include "container.hpp"

/*********************************************
**
** Class Item
**
*********************************************/

Item *Item::items[512] = {nullptr};

Item::Item(int id){
    ID = id;
    items[ID] = this;
}

Item::~Item()
{ }

char Item::onRightClick( World *world, int x, int y, Entity *entity, Perso *perso )
{
    return 0;
}

char Item::onLeftClick( World *world, int x, int y, Entity *entity, Perso *perso )
{
    return 0;
}

void Item::destroy()
{
    delete[] items;
}

Item *Item::getItemFromId( int id )
{
    return items[id];
}


/*********************************************
**
** Class ItemBlock
**
*********************************************/

ItemBlock::ItemBlock( int id ) : Item( id )
{

}

ItemBlock::~ItemBlock()
{

}

char ItemBlock::onRightClick( World *world, int x, int y, Entity *entity, Perso *perso )
{
    if(world->getBlock( x, y, 1 )==Blocks::air)
    {
        world->setBlock( Block::getBlockFromId(ID), x, y, 1 );
        return -1;
    }
    return 0;
}

char ItemBlock::onLeftClick( World *world, int x, int y, Entity *entity, Perso *perso )
{
    if(world->getBlock( x, y, 0 )==Blocks::air)
    {
        world->setBlock( Block::getBlockFromId(ID), x, y, 0 );
        return -1;
    }
    return 0;
}


/*********************************************
**
** Class ItemDematerializer
**
*********************************************/

ItemDematerializer::ItemDematerializer( int id ) : Item(id), state(0)
{
    cursor[0] = 0;
    cursor[1] = 0;
}

ItemDematerializer::~ItemDematerializer()
{

}

char ItemDematerializer::onRightClick( World *world, int x, int y, Entity *entity, Perso *perso )
{
    if(x==cursor[0]&&y==cursor[1])
        state++;
    else
    {
        cursor[0] = x;
        cursor[1] = y;
        state = 0;
    }
    if(state>15)
    {
        if(world->getBlock(x,y,1)!=Blocks::air)
        {
            perso->inventory.addItems(ItemStack(world->getBlock(x,y,1)->getID(),1));
            world->setBlock( Blocks::air, x, y, 1 );
        }
    }
    return 0;
}

char ItemDematerializer::onLeftClick( World *world, int x, int y, Entity *entity, Perso *perso )
{
    if(x==cursor[0]&&y==cursor[1])
        state++;
    else
    {
        cursor[0] = x;
        cursor[1] = y;
        state = 0;
    }
    if(state>15)
    {
        if(world->getBlock( x, y, 0 )!=Blocks::air)
        {
            perso->inventory.addItems(ItemStack(world->getBlock(x,y,0)->getID(),1));
            world->setBlock( Blocks::air, x, y, 0 );
        }
        state = 0;
    }
    return 0;
}


/*********************************************
**
** Class Items
**
*********************************************/

ItemDematerializer *Items::dematerializer = new ItemDematerializer( 256 );
MultiImage *Items::textureItems = nullptr;

void Items::render(int id, int x, int y){
    if(id==0) return;
    else if(id<256)
    {
        Blocks::render( Block::getBlockFromId(id), x, y, 2, 2 );
    }
    else
    {
        int posX = (id-256)%8;
        int posY = (id-256)/8;
        textureItems->render( x, y, 1, 1, posX, posY );
    }
}

void Items::render( int id, int x, int y, int w, int h ){
    if(id==0) return;
    else if(id<256)
    {
        Blocks::render( Block::getBlockFromId(id), x, y, 2*w, 2*h );
    }
    else
    {
        int posX = (id-256)%8;
        int posY = (id-256)/8;
        textureItems->render( x, y, w, h, posX, posY );
    }
}

void Items::destroy(){
    delete textureItems;
    Item::destroy();
}

void Items::initTexture(){
    textureItems = new MultiImage("items.png", 8, 8);
}

/*********************************************
**
** Class ItemStack
**
*********************************************/

ItemStack::ItemStack( int itemId, int quantity )
{
    itemID = itemId;
    nItems = quantity;
}

ItemStack::~ItemStack()
{ }

void ItemStack::render( int x, int y )
{
    Items::render( itemID, x, y );
    drawText( to_string(nItems), "white", 0, NULL, x, y );
}

int ItemStack::addItems( int quantity )
{
    int n = 0;
    nItems += quantity;
    if( nItems>STACKMAX )
    {
        n = nItems - STACKMAX;
        nItems = STACKMAX;
    }
    else if( nItems <= 0 )
    {
        return -1;
    }
    return n;
}

bool ItemStack::onRightClick( World *world, int x, int y, Entity *entity, Perso *perso )
{
    return addItems(Item::getItemFromId( itemID )->onRightClick( world, x, y, entity, perso ))==-1;
}

bool ItemStack::onLeftClick( World *world, int x, int y, Entity *entity, Perso *perso )
{
    return addItems(Item::getItemFromId( itemID )->onLeftClick( world, x, y, entity, perso ))==-1;
}
