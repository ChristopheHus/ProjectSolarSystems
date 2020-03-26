#include "world.hpp"
#include "container.hpp"

/*********************************************
**
** Class Chunk
**
*********************************************/

Chunk::Chunk(){
	for(int j=0; j<256; j++)
		for(int i=0; i<64; i++){
			terrain[0][i][j] = Blocks::air;
			terrain[1][i][j] = Blocks::air;
			lightLvl[i][j].value = 0;
			lightLvl[i][j].side = UP;
			lightLvl[i][j].fromSky = false;
		}
}

Chunk::Chunk( ifstream &file )
{
    unsigned char a;
    for(int y=0;y<256;y++)
        for(int x=0;x<64;x++)
        {
            file.read((char*)&a,1);
            terrain[0][x][y] = Block::getBlockFromId(a);
            file.read((char*)&a,1);
            terrain[1][x][y] = Block::getBlockFromId(a);
            file.read((char*)&lightLvl[x][y].value,1);
            file.read((char*)&lightLvl[x][y].side,1);
            file.read((char*)&a,1);
            lightLvl[x][y].fromSky = a==255;
        }
}

void Chunk::generate( PerlinNoise *noise, int height, int offset, int num ){
	int n[64];
	int a;
	noise->genTerrain( n, height, offset, num, 64 );

	for(int i=0; i<64; i++){
		a = n[i];
		terrain[0][i][a] = Blocks::grass; terrain[1][i][a] = Blocks::grass;
		if(a>=1){
			terrain[0][i][a-1] = Blocks::dirt; terrain[1][i][a-1] = Blocks::dirt;
			if(a>=2){
				terrain[0][i][a-2] = Blocks::dirt; terrain[1][i][a-2] = Blocks::dirt;
				if(a>=3)
				    for(int j=n[i]-3; j>=0; j--)
                    {
						terrain[0][i][j] = Blocks::stone; terrain[1][i][j] = Blocks::stone;
                    }

			}
		}
	}
}

int Chunk::getHeight( int x ){
    int y(255);
    while( (terrain[0][x][y]==Blocks::air) && (y>0) ){
        y--;
    }
    return y;
}

void Chunk::save( ofstream &file ){
    unsigned char a;
    for(int y=0;y<256;y++)
        for(int x=0;x<64;x++)
        {
            a = terrain[0][x][y]->getID();
            file.write((char*)&a,1);
            a = terrain[1][x][y]->getID();
            file.write((char*)&a,1);
            file.write((char*)&lightLvl[x][y].value,1);
            file.write((char*)&lightLvl[x][y].side,1);
            a = 255 * lightLvl[x][y].fromSky;
            file.write((char*)&a,1);
        }
}

/*********************************************
**
** Class World
**
*********************************************/


World::World( long seed, int height, int offset ){
    srand( seed );
	this->height = height;
	this->offset = offset;
	for(int i=0; i<256; i++)
        chunks[i] = new Chunk();
    blockShape = { 0, 0, 16, 16 };
}

World::World( ifstream &file )
{
    blockShape = { 0, 0, 16, 16 };
    unsigned char a(0),b(0);
    file.read((char*)&a,1);
    height = (int)a;
    file.read((char*)&a,1);
    offset = (int)a;
    for(int i=0;i<256;i++)
        chunks[i] = new Chunk( file );
    file.read((char*)&b,1);
    for(int i=0;i<b;i++)
    {
        guis.push_back( new WorldGui() );
        file.read((char*)&a,1);
        file.read((char*)&guis[i]->x,4);
        file.read((char*)&guis[i]->y,4);
        switch(a)
        {
        case 1:
            guis[i]->gui = new Chest( file );
            break;
        }
    }
}

World::~World(){
    delete[] chunks;
}

void World::generate(){
    for(int i=0;i<256;i++)
        chunks[i]->generate( &noise, height, offset, i );
    initLight();
    for(int i=0;i<256;i++)
        populate(i);
}

void World::populate( int num ){
    //Gen trees
    int nTrees(rand()%16);
    int x,y;
    for(int i=0; i<nTrees; i++){
        x = rand()%64 + num*64;
        y = getHeight( x );
        if(x>1)
        {
        while(getBlock(x,y,0)->isBlockTransparent())
            y--;
        genTree( x, y+1 );
        }
    }

    //Gen ores
    genOre(Blocks::coalOre, num*64, num*64+63,100,50);
    genOre(Blocks::copperOre, num*64, num*64+63,80,30);
    genOre(Blocks::ironOre, num*64, num*64+63,60,16);
    genOre(Blocks::silverOre, num*64, num*64+63,30,8);
    genOre(Blocks::goldOre, num*64, num*64+63,30,7);
    genOre(Blocks::platinumOre, num*64, num*64+63,20,3);
    genOre(Blocks::leadOre, num*64, num*64+63,40,13);
    genOre(Blocks::diamondOre, num*64, num*64+63,20,3);
    genOre(Blocks::graphiteOre, num*64, num*64+63,70,15);
}

void World::genTree( int x, int y ){
    int h(rand()%4+4);
    for(int i=0; i<h; i++){
        setBlock( Blocks::wood, x, y+i, 0 );
        if(h-i<2){
            if(getBlock(x-1,y+i,0)==Blocks::air)setBlock( Blocks::leaf, x-1, y+i, 0 );
            if(getBlock(x+1,y+i,0)==Blocks::air)setBlock( Blocks::leaf, x+1, y+i, 0 );
        }else if(h-i<4){
            if(getBlock(x-2,y+i,0)==Blocks::air)setBlock( Blocks::leaf, x-2, y+i, 0 );
            if(getBlock(x-1,y+i,0)==Blocks::air)setBlock( Blocks::leaf, x-1, y+i, 0 );
            if(getBlock(x+1,y+i,0)==Blocks::air)setBlock( Blocks::leaf, x+1, y+i, 0 );
            if(getBlock(x+2,y+i,0)==Blocks::air)setBlock( Blocks::leaf, x+2, y+i, 0 );
        }
    }
    if(getBlock(x-1,y+h,0)==Blocks::air)setBlock( Blocks::leaf, x-1, y+h, 0 );
    if(getBlock(x,y+h,0)==Blocks::air)setBlock( Blocks::leaf, x, y+h, 0 );
    if(getBlock(x+1,y+h,0)==Blocks::air)setBlock( Blocks::leaf, x+1, y+h, 0 );
}

void World::genOre( Block *ore, int xmin, int xmax, int ymax, int n ){
    int oreX;
    int oreY;
    for(int a=0;a<n;a++)
    {
        oreX = rand()%(xmax-xmin+1)+xmin;
        oreY = rand()%ymax;
        //Centre
        if( rand()%10<8 && getBlock(oreX,oreY,1)==Blocks::stone) setBlock(ore,oreX,oreY,1);
        //Cotes
        if( rand()%10<5 && getBlock(oreX-1,oreY,0)==Blocks::stone) setBlock(ore,oreX-1,oreY,0);
        if( rand()%10<5 && getBlock(oreX,oreY+1,0)==Blocks::stone) setBlock(ore,oreX,oreY+1,0);
        if( rand()%10<5 && getBlock(oreX+1,oreY,0)==Blocks::stone) setBlock(ore,oreX+1,oreY,0);
        if( rand()%10<5 && getBlock(oreX,oreY-1,0)==Blocks::stone) setBlock(ore,oreX,oreY-1,0);
        //Coins
        if( rand()%10<3 && getBlock(oreX-1,oreY-1,0)==Blocks::stone) setBlock(ore,oreX-1,oreY-1,0);
        if( rand()%10<3 && getBlock(oreX-1,oreY+1,0)==Blocks::stone) setBlock(ore,oreX-1,oreY+1,0);
        if( rand()%10<3 && getBlock(oreX+1,oreY+1,0)==Blocks::stone) setBlock(ore,oreX+1,oreY+1,0);
        if( rand()%10<3 && getBlock(oreX+1,oreY-1,0)==Blocks::stone) setBlock(ore,oreX+1,oreY-1,0);
    }
}

void World::setBlock( Block *block, int x, int y, int ground ){
    if(block==Blocks::chest&&ground) ground = 0;
	if(x<0)x+=16384;
    else if(x>16383)x-=16384;
	if(y<0 || y>=256)return;
	int nChunk = x/64;
	chunks[nChunk]->setBlock( block, x-nChunk*64, y, ground );
	if(ground) return;
	if(block==Blocks::chest)
	{
        guis.push_back( new WorldGui(new Chest(36),x,y) );
	}
	else if(block==Blocks::air)
    {
        int pos = getWorldGuiAt( x, y );
        if(pos!=-1)
        {
            delete guis[pos]->gui;
            guis.erase( guis.begin() + pos );
        }
    }
    updateLight(x,y,block->isBlockTransparent());
}

Block *World::getBlock( int x, int y, int ground ){
    if(x<0)x+=16384;
    else if(x>16383)x-=16384;
	if(y<0 || y>=256)return 0;
	int nChunk = x/64;
	return chunks[nChunk]->getBlock( x-nChunk*64, y, ground );
}

int World::getHeight( int x ){
	int nChunk = x/64;
	return chunks[nChunk]->getHeight( x-nChunk*64 );
}

Light World::getLight( int const &x, int const &y )
{
    int xx = (x+16384)%16384;
    int nChunk = xx/64;
    return chunks[nChunk]->getLightAt( xx-nChunk*64, y );
}

void World::setLight( int const &x, int const &y, char const &lvl, char const &side, bool const &fromSky )
{
    int xx = (x+16384)%16384;
    int nChunk = xx/64;
    chunks[nChunk]->setLightAt( xx-nChunk*64, y, lvl, side, fromSky );
}

void World::render( int x, int y ){
    SDL_SetRenderDrawBlendMode( getrenderer(), SDL_BLENDMODE_BLEND );
    int a(0), b(0);
    for(int j=y, l=0; j<y+33; j++, l++)
        for(int i=x, k=0; i<x+49; i++, k++){
            a=i,b=j;
            if(a<0) a+=16384;
            if(a>16383) a-=16384;/// >32767 32768
            if(b<0)
                Blocks::render( Blocks::lava, k*16, SCREEN_HEIGHT-l*16 );
            else
            {
                if( getBlock(a,b,0)!=Blocks::air )
                    Blocks::render( getBlock(a,b,0), k*16, SCREEN_HEIGHT-l*16 );
                else if( getBlock( a, b, 1 )!=Blocks::air )
                {
                    Blocks::render( getBlock(a,b,1), k*16, SCREEN_HEIGHT-l*16 );
                    blockShape.x = k*16;  blockShape.y = SCREEN_HEIGHT-l*16;
                    SDL_SetRenderDrawColor( getrenderer(), 0, 0, 0, 127 );
                    SDL_RenderFillRect( getrenderer(), &blockShape );
                }
                if(getLight(a,b).value<15)
                {
                    blockShape.x = k*16;  blockShape.y = SCREEN_HEIGHT-l*16;
                    SDL_SetRenderDrawColor( getrenderer(), 0, 0, 0, 16*(15-getLight(a,b).value) );
                    SDL_RenderFillRect( getrenderer(), &blockShape );
                }
            }
        }
}

void World::initLight()
{
    for(int i=0;i<16384;i++)
    {
        int y(255);
        while(getBlock(i,y,0)->isBlockTransparent())
        {
            setLight( i, y, 15, UP, true );
            y--;
        }
        setLight( i, y, 15, UP, true );
    }
}

void World::updateLight( int const &x, int const &y, bool isAir )
{
    Light a;
    a = getLight(x,y);

    if(isAir)
    {
        if((!(a.side==UP)) && (getLight(x,y+1).value < a.value-1))
        {
            setLight(x,y+1,a.value-1,DOWN,false);
            if(getBlock(x,y+1,0)->isBlockTransparent())
                updateLight(x,y+1,true);
        }
        if(!(a.side==DOWN))
        {
            if(a.fromSky)
            {
                setLight(x,y-1,15,UP,true);
                if(getBlock(x,y-1,0)->isBlockTransparent())
                    updateLight(x,y-1,true);
            }
            else if(getLight(x,y-1).value < a.value-1)
            {
                setLight(x,y-1,a.value-1,UP,false);
                if(getBlock(x,y-1,0)->isBlockTransparent())
                    updateLight(x,y-1,true);
            }
        }
        if((!(a.side==LEFT)) && (getLight(x-1,y).value<a.value-1))
        {
            setLight(x-1,y,a.value-1,RIGHT,false);
            if(getBlock(x-1,y,0)->isBlockTransparent())
                updateLight(x-1,y,true);
        }
        if((!(a.side==RIGHT)) && (getLight(x+1,y).value<a.value-1))
        {
            setLight(x+1,y,a.value-1,LEFT,false);
            if(getBlock(x+1,y,0)->isBlockTransparent())
                updateLight(x+1,y,true);
        }
        return;
    }
    else
    {
        if(!(a.side==UP))
        {
            if(getLight(x,y+1).side==DOWN)
            {
                setLight(x,y+1,0,DOWN,false);
                Light b;

                b = getLight(x,y+2);
                if(getBlock(x,y+2,0)!=nullptr)
                    if((b.side!=DOWN) && (b.value>getLight(x,y+1).value) && (getBlock(x,y+2,0)->isBlockTransparent()) )
                        setLight(x,y+1,b.value-1*(!b.fromSky),UP,b.fromSky);

                b = getLight(x-1,y+1);
                if(getBlock(x-1,y+1,0)!=nullptr)
                    if((b.side!=RIGHT) && (b.value>getLight(x,y+1).value) && (getBlock(x-1,y+1,0)->isBlockTransparent()) )
                        setLight(x,y+1,b.value,LEFT,false);

                b = getLight(x+1,y+1);
                if(getBlock(x+1,y+1,0)!=nullptr)
                    if((b.side!=LEFT) && (b.value>getLight(x,y+1).value) && (getBlock(x+1,y+1,0)->isBlockTransparent()) )
                        setLight(x,y+1,b.value,RIGHT,false);

                if(getBlock(x,y+1,0)!=nullptr)
                    if(getBlock(x,y+1,0)->isBlockTransparent())
                        updateLight(x,y+1,false);
            }
        }
        if(!(a.side==DOWN))
        {
            if(getLight(x,y-1).side==UP)
            {
                setLight(x,y-1,0,UP,false);
                Light b;

                b = getLight(x,y-2);
                if(getBlock(x,y-2,0)!=nullptr)
                    if((b.side!=UP) && (b.value>getLight(x,y-1).value) && (getBlock(x,y-2,0)->isBlockTransparent()) )
                        setLight(x,y-1,b.value-1,DOWN,false);

                b = getLight(x-1,y-1);
                if(getBlock(x-1,y-1,0)!=nullptr)
                    if((b.side!=RIGHT) && (b.value>getLight(x,y-1).value) && (getBlock(x-1,y-1,0)->isBlockTransparent()) )
                            setLight(x,y-1,b.value-1,LEFT,false);

                b = getLight(x+1,y-1);
                if(getBlock(x+1,y-1,0)!=nullptr)
                    if((b.side!=LEFT) && (b.value>getLight(x,y-1).value) && (getBlock(x+1,y-1,0)->isBlockTransparent()) )
                        setLight(x,y-1,b.value-1,RIGHT,false);

                if(getBlock(x,y-1,0)!=nullptr)
                    if(getBlock(x,y-1,0)->isBlockTransparent())
                        updateLight(x,y-1,false);
            }
        }
        if(!(a.side==LEFT))
        {
            if(getLight(x-1,y).side==RIGHT)
            {
                setLight(x-1,y,0,RIGHT,false);
                Light b;

                b = getLight(x-1,y-1);
                if(getBlock(x-1,y-1,0)!=nullptr)
                    if((b.side!=UP) && (b.value>getLight(x-1,y).value) && (getBlock(x-1,y-1,0)->isBlockTransparent()) )
                        setLight(x-1,y,b.value-1,DOWN,false);

                b = getLight(x-2,y);
                if(getBlock(x-2,y,0)!=nullptr)
                    if((b.side!=RIGHT) && (b.value>getLight(x-1,y).value) && (getBlock(x-2,y,0)->isBlockTransparent()) )
                        setLight(x-1,y,b.value-1,LEFT,false);

                b = getLight(x-1,y+1);
                if(getBlock(x-1,y+1,0)!=nullptr)
                    if((b.side!=DOWN) && (b.value>getLight(x-1,y).value) && (getBlock(x-1,y+1,0)->isBlockTransparent()) )
                        setLight(x-1,y,b.value-1,UP,false);

                if(getBlock(x-1,y,0)!=nullptr)
                    if(getBlock(x-1,y,0)->isBlockTransparent())
                        updateLight(x-1,y,false);
            }
        }
        if(!(a.side==RIGHT))
        {
            if(getLight(x+1,y).side==LEFT)
            {
                setLight(x+1,y,0,LEFT,false);
                Light b;

                b = getLight(x+1,y-1);
                if(getBlock(x+1,y-1,0)!=nullptr)
                    if((b.side!=UP) && (b.value>getLight(x+1,y).value) && (getBlock(x+1,y-1,0)->isBlockTransparent()) )
                        setLight(x+1,y,b.value-1,DOWN,false);

                b = getLight(x+2,y);
                if(getBlock(x+2,y,0)!=nullptr)
                    if((b.side!=LEFT) && (b.value>getLight(x+1,y).value) && (getBlock(x+2,y,0)->isBlockTransparent()) )
                        setLight(x+1,y,b.value-1,RIGHT,false);

                b = getLight(x+1,y+1);
                if(getBlock(x+1,y+1,0)!=nullptr)
                    if((b.side!=DOWN) && (b.value>getLight(x+1,y).value) && (getBlock(x+1,y+1,0)->isBlockTransparent()) )
                        setLight(x+1,y,b.value-1,UP,false);

                if(getBlock(x+1,y,0)!=nullptr)
                    if(getBlock(x+1,y,0)->isBlockTransparent())
                        updateLight(x+1,y,false);
            }
        }
    }
}

void World::save( ofstream &file )
{
    unsigned char a(0);
    a = height;
    file.write((char*)&a,1);
    a = offset;
    file.write((char*)&a,1);
    for(int i=0;i<256;i++)
        chunks[i]->save( file );
    a = guis.size();
    file.write((char*)&a,1);
    for(int i=0;i<guis.size();i++)
    {
        a = guis[i]->gui->getGuiType();
        file.write((char*)&a,1);
        file.write((char*)&guis[i]->x,4);
        file.write((char*)&guis[i]->y,4);
        guis[i]->gui->save( file );
    }
}

Gui *World::getGuiAt( int x, int y )
{
    for(int i=0;i<guis.size();i++)
    {
        if(guis[i]->x==x&&guis[i]->y) return guis[i]->gui;
    }
    return nullptr;
}

int World::getWorldGuiAt( int x, int y )
{
    for(int i=0;i<guis.size();i++)
    {
        if(guis[i]->x==x&&guis[i]->y) return i;
    }
    return -1;
}
