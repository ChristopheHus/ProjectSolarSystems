#ifndef ITEM_INCLUDED
#define ITEM_INCLUDED

#include "block.hpp"

class Item
{
public:
  Item();
  ~Item();
protected:
  char name[16];
  unsigned char ID;
  static Item *items[512];
};

class ItemBlock : public Item
{
public:
  ItemBlock( Block *block )
  ~ItemBlock();

};

class ItemStack
{
public:
  ItemStack( Item *item );
private:
  int quantity;
  Item *item;
};

#endif // ITEM_INCLUDED
