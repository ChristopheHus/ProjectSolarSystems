#ifndef BLOCK_INCLUDED
#define BLOCK_INCLUDED

class ItemBlock;

class Block
{
public:
  Block( int const &id, bool const &transparent, char const &lightEmitted, int const &hardness, int const &breakingTool );
  ~Block();
  int getID() { return ID; }
  bool isTransparent() { return m_tansparent; }
  char getLightEmitted() { return m_emitLight; }
  int getHardness() { return m_hardness; }
  int getBreakingTool() { return m_breakingTool; }
  char[] getName() { return name; }
  static void initBlock();
  static void destroyBlock();
  static Block *getBlock( int id ) { return blocks[id]; }
  // + render
protected:
  char name[16];
  unsigned char ID;
  bool m_transparent;
  char m_emitLight;
  unsigned char m_hardness;
  unsigned char m_breakingTool;
  ItemBlock *itemblock;
  static Block *blocks[256];
};

#endif // BLOCK_INCLUDED
