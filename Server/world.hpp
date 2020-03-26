#ifndef WORLD_INCLUDED
#define WORLD_INCLUDED

#include "defs.hpp"
#include "noise.hpp"
#include "block.hpp"

class Chunk
{
public:
  Chunk();
  void genTerrain( int terrain[], int const &x );
};

class World
{
public:
  World( int const &nbChunks, int const &height, int const &offset );
  ~World();
  void genWorld();
  void setBlock( Block *block, int const &x, int const &y );
  Block *getBlock( int const &x, int const &y );
};

#endif // WORLD_INCLUDED
