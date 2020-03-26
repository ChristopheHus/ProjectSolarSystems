#include "block.hpp"
#include "item.hpp"

/*********************************************
**
** Class Block
**
*********************************************/

Block *Block::blocks[256] = {nullptr};


Block::Block( int id, bool transparency ){
	ID = id;
    blocks[ID] = this;
    isTransparent = transparency;
    ItemBlock *item = new ItemBlock(ID);
}

Block::~Block(){
}

void Block::destroy()
{
    delete[] blocks;
}

Block *Block::getBlockFromId( int id )
{
    return blocks[id];
}

/*********************************************
**
** Class Blocks
**
*********************************************/

Block *Blocks::air = new Block(0,true);
Block *Blocks::grass = new Block(1,false);
Block *Blocks::dirt = new Block(2,false);
Block *Blocks::stone = new Block(3,false);
Block *Blocks::wood = new Block(4,false);
Block *Blocks::leaf = new Block(5,true);
Block *Blocks::coalOre = new Block(6,false);
Block *Blocks::lava = new Block(7,false);
Block *Blocks::copperOre = new Block(8,false);
Block *Blocks::ironOre = new Block(9,false);
Block *Blocks::silverOre = new Block(10,false);
Block *Blocks::goldOre = new Block(11,false);
Block *Blocks::platinumOre = new Block(12,false);
Block *Blocks::leadOre = new Block(13,false);
Block *Blocks::diamondOre = new Block(14,false);
Block *Blocks::graphiteOre = new Block(15,false);
Block *Blocks::chest = new Block(16,false);
Block *Blocks::limeStone = new Block(17,false);
Block *Blocks::basalt = new Block(18,false);
Block *Blocks::dirtyStone = new Block(19,false);
Block *Blocks::pinkyStone = new Block(20,false);
Block *Blocks::greenyStone = new Block(21,false);
MultiImage *Blocks::textureBlocks = nullptr;

void Blocks::render( Block *block, int x, int y ){
    if(block==nullptr) return;
    int id = block->getID();
    int posX = (id-1)%16;
    int posY = (id-1)/16;
    textureBlocks->render( x, y, 1, 1, posX, posY );
}

void Blocks::render( Block *block, int x, int y, int w, int h ){
    int id = block->getID();
    int posX = (id-1)%16;
    int posY = (id-1)/16;
    textureBlocks->render( x, y, w, h, posX, posY );
}

void Blocks::destroy(){
    delete textureBlocks;
    Block::destroy();
}

void Blocks::initTexture(){
    textureBlocks = new MultiImage( "blocks.png", 16, 16 );
}
