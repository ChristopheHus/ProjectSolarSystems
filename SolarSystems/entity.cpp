#include "entity.hpp"

/*********************************************
**
** Class Entity
**
*********************************************/

Entity::Entity( int health, int x, int y )
{
    hpMax = health;
    hp = (float)hpMax;
    a[0] = 0;
    a[1] = 0;
    v[0] = 0;
    v[1] = 0;
    pos[0] = x*16;
    pos[1] = y*16;
    isInvincible = false;
}

Entity::Entity(  int health, int x, int y, bool invincibility )
{
    hpMax = health;
    hp = (float)hpMax;
    a[0] = 0;
    a[1] = 0;
    v[0] = 0;
    v[1] = 0;
    pos[0] = x*16;
    pos[1] = y*16;
    isInvincible = invincibility;
}

Entity::~Entity()
{
    delete texture;
}

void Entity::takeDamage( Entity *aggressor, float damages )
{
    if(isInvincible) return;
    hp -= damages;
    if(hp<0) hp = 0;
}

void Entity::render( int x, int y )
{
    texture->render( x, y );
}

void Entity::update( World *world )
{
    //Acceleration
    if(motion[0]) a[0] = -5;
    else if(motion[1]) a[0] = 5;
    else { a[0] = 0; v[0] = 0; }
    if( (world->getBlock(pos[0]/16,pos[1]/16-1,0)==Blocks::air) && (world->getBlock(pos[0]/16+1,pos[1]/16-1,0)==Blocks::air) ) a[1] = -2;
    else if(motion[2]) a[1] = 3;

    //Vitesse
    v[0] += a[0];
    v[1] += a[1];
    if(v[0]>16)v[0]=16;
    if(v[0]<-16)v[0]=-16;

    //Position
    if(motionX(world,v[0]))
    {
        a[0] = 0;
        v[0] = 0;
    }

    if(motionY(world,v[1]))
    {
        a[1] = 0;
        v[1] = 0;
    }

    //Reset des touches
    for(int i=0; i<3; i++)
        motion[i] = false;

    if(pos[0]>262143) pos[0] -= 262144;
    else if(pos[0]<0) pos[0] += 262144;

    if(pos[1]<0)pos[1]=0;
}

bool Entity::motionX( World *world, int x )
{
    if(x>0)
    {
        for(int i=1;i<=x;i++)
        {
            if(((pos[0]+32)%16)==0)
            {
                if( world->getBlock((pos[0]+32)/16,pos[1]/16+1,0)!=Blocks::air || world->getBlock((pos[0]+32)/16,pos[1]/16+2,0)!=Blocks::air || world->getBlock((pos[0]+32)/16,pos[1]/16+3,0)!=Blocks::air )
                    return true;
                else if( world->getBlock((pos[0]+32)/16,pos[1]/16,0)!=Blocks::air )
                {
                    if( world->getBlock((pos[0]+32)/16-2,pos[1]/16+4,0)!=Blocks::air || world->getBlock((pos[0]+32)/16-1,pos[1]/16+4,0)!=Blocks::air || world->getBlock((pos[0]+32)/16,pos[1]/16+4,0)!=Blocks::air )
                        return true;
                    else { pos[0]++; pos[1]+=16; }
                }
                else pos[0]++;
            }
            else pos[0]++;
        }
        return false;
    }
    else if(x<0)
    {
        for(int i=1;i<=-x;i++)
        {
            if(((pos[0]-1)%16)==15)
            {
                if( world->getBlock(pos[0]/16-1,pos[1]/16+1,0)!=Blocks::air || world->getBlock(pos[0]/16-1,pos[1]/16+2,0)!=Blocks::air || world->getBlock(pos[0]/16-1,pos[1]/16+3,0)!=Blocks::air )
                    return true;
                else if( world->getBlock((pos[0]-1)/16,pos[1]/16,0)!=Blocks::air )
                {
                    if( world->getBlock((pos[0])/16+1,pos[1]/16+4,0)!=Blocks::air || world->getBlock(pos[0]/16,pos[1]/16+4,0)!=Blocks::air || world->getBlock(pos[0]/16-1,pos[1]/16+4,0)!=Blocks::air )
                        return true;
                    else { pos[0]++; pos[1]+=16; }
                }
                else pos[0]--;
            }
            else pos[0]--;
        }
        return false;
    }
    return false;
}

bool Entity::motionY( World *world, int y )
{
    if(y>0)
    {
        for(int j=1;j<=y;j++)
        {
            if(((pos[1]+64)%16)==0)
            {
                if( world->getBlock(pos[0]/16,(pos[1]+64)/16,0)!=Blocks::air || world->getBlock(pos[0]/16+1,(pos[1]+64)/16,0)!=Blocks::air || ( world->getBlock(pos[0]/16+2,(pos[1]+64)/16,0)!=Blocks::air && pos[0]%16 ) )
                    return true;
                else pos[1]++;
            }
            else pos[1]++;
        }
        return false;
    }
    else if(y<0)
    {
        for(int j=1;j<=-y;j++)
        {
            if(((pos[1]-1)%16)==15)
            {
                if( world->getBlock(pos[0]/16,(pos[1]-1)/16,0)!=Blocks::air || world->getBlock(pos[0]/16+1,(pos[1]-1)/16,0)!=Blocks::air || ( world->getBlock(pos[0]/16+2,(pos[1]-1)/16,0)!=Blocks::air && pos[0]%16 ) )
                    return true;
                else pos[1]--;
            }
            else pos[1]--;
        }
        return false;
    }
    return false;
}
