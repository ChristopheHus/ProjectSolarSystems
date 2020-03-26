#include "block.hpp"
#include "item.hpp"

Block *Block::blocks[256] = { nullptr };

Block::Block( int const &id, bool const &transparent, char const &lightEmitted, int const &hardness, int const &breakingTool )
{
  ID = id;
  m_transparent = transparent;
  m_emitLight = lightEmitted;
  m_hardness = hardness;
  m_breakingTool = breakingTool;
  blocks[id] = this;
  itemblock = new ItemBlock( this );
}

Block::~Block()
{ }

void Block::initBlock()
{
  //load block file
}

void Block::destroyBlock()
{
  delete[] blocks;
}
