#ifndef PERSO
#define PERSO

#include "entity.hpp"
#include "container.hpp"

class Perso : public Entity {
public:
    Perso( int x, int y );
    Perso( ifstream &file );
    void save( ofstream &file );
    void renderLife( float sizeX, float sizeY );
    Inventory inventory;
};

#endif // PERSO
