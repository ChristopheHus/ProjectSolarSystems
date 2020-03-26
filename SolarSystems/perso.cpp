#include "perso.hpp"

/*********************************************
**
** Class Perso
**
*********************************************/

Perso::Perso( int x, int y ): Entity( 20, x, y ), inventory( ) {
    //if( texture != nullptr )
    //    delete texture;
    texture = new Image( "perso.png" );

}

Perso::Perso( ifstream &file ) : Entity( 20, 0, 0 ), inventory( file )
{
    texture = new Image( "perso.png" );
    file.read((char*)&pos[0],4);
    file.read((char*)&pos[1],4);
    file.read((char*)&hp,4);
}

void Perso::save( ofstream &file )
{
    inventory.save( file );
    file.write((char*)&pos[0],4);
    file.write((char*)&pos[1],4);
    file.write((char*)&hp,4);
}

void Perso::renderLife( float sizeX, float sizeY )
{
    SDL_SetRenderDrawColor( getrenderer(), 200, 25, 0, 255 );
    for(int j=10*sizeY;j<21*sizeY;j++)
        for(int i=567*sizeX;i<758*sizeX;i++)
            SDL_RenderDrawPoint( getrenderer(), i, j );
}
