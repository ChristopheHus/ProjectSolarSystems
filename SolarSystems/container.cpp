#include "container.hpp"
#include "perso.hpp"

/*********************************************
**
** Class Container
**
*********************************************/

Container::Container( int n )
{
    for(int i=0;i<n;i++)
    {
        slots.push_back(nullptr);
    }
}

Container::Container( ifstream &file )
{
    int x;
    file.read((char*)&x,4);
    short int id(0),q(0);
    for( int i=0; i<x; i++)
    {
        file.read((char*)&id,2);
        file.read((char*)&q,2);
        if(id!=0&&q!=0)
            slots.push_back( new ItemStack(id,q) );
        else
            slots.push_back( nullptr );
    }
}

Container::~Container()
{
    for(int i=0; i<slots.size();i++)
        delete slots[i];
}

int Container::addItems( ItemStack items )
{
    if(items.getQuantity()>99) return items.getQuantity();
    int quantity( items.getQuantity() );

    //On essaye de caser les items dans des stacks déjà existants
    for(int i=0; i<slots.size(); i++)
    {
        if( slots[i] != nullptr )
            if( slots[i]->getId() == items.getId() )
            {
                quantity = slots[i]->addItems(items.getQuantity());
            }
    }

    //Si il reste des items non cases, on essaye de creer un nouveau stack d'un emplacement libre
    if( quantity>0 )
        for( int j=0; j<slots.size(); j++ )
        {
            if( slots[j] == nullptr )
            {
                slots[j] = new ItemStack(items.getId(),quantity);
                return 0;
            }
        }

    return quantity;
}

void Container::save( ofstream &file )
{
    int x = slots.size();
    file.write((char*)&x,4);
    short int id, q;
    for( int i=0; i<slots.size(); i++)
    {
        if(slots[i]==nullptr)
        {
            id = 0;
            q = 0;
        }
        else
        {
            id = slots[i]->getId();
            q = slots[i]->getQuantity();
        }
        file.write((char*)&id,2);
        file.write((char*)&q,2);
    }
}


/*********************************************
**
** Class Container
**
*********************************************/

Gui::Gui( int n ) : Container( n ), buttons(Button(768,512))
{
    guiType = 0;
}

Gui::Gui( ifstream &file ) : Container( file ), buttons(Button(768,512))
{
    guiType = 0;
}

Gui::~Gui()
{

}

ItemStack *Gui::exchangeSlots( ItemStack* slot, int n )
{
    if(n<0||n>slots.size()) return nullptr;
    ItemStack *buf = nullptr;
    buf = slots[n];
    slots[n] = slot;
    return buf;
}

void Gui::exchangeSlots( int n1, int n2 )
{
    if((n1<0||n1>slots.size()) || (n2<0||n2>slots.size())) return;
    ItemStack *buf = slots[n1];
    slots[n1] = slots[n2];
    slots[n2] = buf;
}


/*********************************************
**
** Class Chest
**
*********************************************/

Chest::Chest( int length ) : Gui( length )
{
    inventory = new Image( "inventory.png" );
    for(int i=0;i<length;i++)
        buttons.newButton(i+1,216+(i%9)*38,72+(i/9)*38,32,32);
    guiType = 1;
}

Chest::Chest( ifstream &file ) : Gui( file )
{
    inventory = new Image( "inventory.png" );
    for(int i=0;i<slots.size();i++)
        buttons.newButton(i+1,216+(i%9)*38,72+(i/9)*38,32,32);
    guiType = 1;
}

Chest::~Chest()
{
    delete inventory;
    for(int i=0; i<slots.size();i++)
        delete slots[i];
}

void Chest::render()
{
    SDL_Rect voidSlot = { 0, 0, 32, 32 };
    SDL_SetRenderDrawColor( getrenderer(), 64, 64, 64, 255 );
    inventory->render( 208, 64 );
    for(int i=0;i<36;i++)
    {
        voidSlot.x = 216+(i%9)*38;
        voidSlot.y = 72+(i/9)*38;
        if(i<slots.size())
        {
            if(slots[i]!=nullptr)
            {
                slots[i]->render( voidSlot.x, voidSlot.y );
            }
        }
        else
        {
            SDL_RenderFillRect( getrenderer(), &voidSlot );
        }
    }
}

int Chest::getPointedSlot( int x, int y )
{
    return buttons.clickAt(x,y);
}

void Chest::getRectAt( SDL_Rect *rect, int slot )
{
    rect->x = (slot%9)*38+215;
    rect->y = (slot/9)*38+71;
}


/*********************************************
**
** Class Inventory
**
*********************************************/

Inventory::Inventory() : Container(45), inv(Button(768,512))
{
    selection = 0;
    selectedStack = -1;
    select = { 215, 461, 34, 34 };
    slot = { 0, 462, 32, 32 };
    mSelect = { 0, 0, 34, 34 };
    inventory = new Image( "inventory.png" );
    buffer = SDL_CreateTexture( getrenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT );

    for(int i=0;i<9;i++)
        inv.newButton(i+1,216+i*38,462,32,32);
    for(int j=0;j<4;j++)
        for(int i=0;i<9;i++)
            inv.newButton(i+j*9+10,216+i*38,272+j*38,32,32);
}

Inventory::Inventory( ifstream &file ) : Container(file), inv(Button(768,512))
{
    selection = 0;
    selectedStack = -1;
    select = { 215, 461, 34, 34 };
    slot = { 0, 462, 32, 32 };
    mSelect = { 0, 0, 34, 34 };
    inventory = new Image( "inventory.png" );
    buffer = SDL_CreateTexture( getrenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT );

    for(int i=0;i<9;i++)
        inv.newButton(i+1,216+i*38,462,32,32);
    for(int j=0;j<4;j++)
        for(int i=0;i<9;i++)
            inv.newButton(i+j*9+10,216+i*38,272+j*38,32,32);
}

Inventory::~Inventory()
{
    delete inventory;
    SDL_DestroyTexture(buffer);
    for(int i=0; i<slots.size();i++)
        delete slots[i];
}

void Inventory::render( bool inInv )
{
    if(inInv)
    {
        inventory->render( 208, 264 );

        for( int j=0; j<4; j++ )
        {
            for( int i=0; i<9; i++ )
            {
                if( slots[9+i+j*9] ) slots[9+i+j*9]->render( 216+i*38, 272+j*38 );
            }
        }
        SDL_SetRenderDrawColor( getrenderer(), 128, 128, 128, 255 );
        for(int i=0;i<9;i++)
        {
            slot.x = 216 + 38*i;
            SDL_RenderFillRect( getrenderer(), &slot );
            if( slots[i] != nullptr ) slots[i]->render( slot.x, slot.y );
        }

        if(selectedStack!=-1){
            SDL_SetRenderDrawColor( getrenderer(), 160, 235, 240, 255 );
            SDL_RenderDrawRect( getrenderer(), &mSelect );
        }
    }
    else
    {
        SDL_SetRenderDrawColor( getrenderer(), 255, 255, 160, 255 );
        SDL_RenderDrawRect( getrenderer(), &select );

        for(int i=0;i<9;i++)
            if( slots[i] != nullptr ) slots[i]->render( 216 + i*38, 462 );
    }
}

void Inventory::update( int x, int y )
{
    if(selectedStack>=45) selectedStack = -1;
    ItemStack *buf = nullptr;
    int s=inv.clickAt(x,y)-1;
    if(s!=-1)
    {
        if(selectedStack==-1)
        {
            selectedStack = s;
            mSelect.x = 215+(s%9)*38;
            mSelect.y = 461*(s<9)+(271+((s/9-1)*38))*(s>=9);
        }
        else if(selectedStack!=s)
        {
            //if(event.button.button==SDL_BUTTON_LEFT)
                buf = slots[selectedStack];
                slots[selectedStack] = slots[s];
                slots[s] = buf;
                selectedStack = -1;
            /**else if(SDL_BUTTON_RIGHT)
            {
                Stack/2
                mouseStack = slots[s];
                slots[s] = buf;
            }**/
        }
        else { selectedStack = -1; }
    }
    else { selectedStack = -1; }
}

void Inventory::render( Gui *gui )
{
    gui->render();
    this->render();
}

void Inventory::update( int x, int y, Gui *gui )
{
    int s = gui->getPointedSlot(x,y)-1;
    if(s!=-1)
    {
        if(selectedStack==-1)
        {
            selectedStack = s+45;
            gui->getRectAt( &mSelect, s );
        }
        else if(selectedStack!=s)
        {
            if(selectedStack>=45)
                gui->exchangeSlots( selectedStack-45, s );
            else
            {
                slots[selectedStack] = gui->exchangeSlots( slots[selectedStack], s );
            }
            selectedStack = -1;
        }
        else { selectedStack = -1; }
    }
    else
    {
        s = inv.clickAt(x,y)-1;
        if(s!=-1)
        {
            if(selectedStack==-1)
            {
                selectedStack = s;
                mSelect.x = 215+(s%9)*38;
                mSelect.y = 461*(s<9)+(271+((s/9-1)*38))*(s>=9);
            }
            else if(selectedStack!=s)
            {
                if(selectedStack>=45)
                {
                    slots[s] = gui->exchangeSlots( slots[s], selectedStack-45 );
                }
                else
                {
                    ItemStack *buf = nullptr;
                    buf = slots[selectedStack];
                    slots[selectedStack] = slots[s];
                    slots[s] = buf;
                }
                selectedStack = -1;
            }
            else { selectedStack = -1; }
        }
        else { selectedStack = -1; }
    }
}

void Inventory::changeSelection( int offset )
{
    selection += offset;
    while( selection<0 ) selection += 9;
    selection %= 9;
    select.x = 215 + selection*38;
}

void Inventory::onRightClick( World *world, int x, int y, Entity *entity, Perso *perso )
{
    if( slots[selection] != nullptr )
    {
        if(slots[selection]->onRightClick( world, x, y, entity, perso ))
        {
            delete slots[selection];
            slots[selection] = nullptr;
        }
    }
}

void Inventory::onLeftClick( World *world, int x, int y, Entity *entity, Perso *perso )
{
    if( slots[selection] != nullptr )
    {
        if(slots[selection]->onLeftClick( world, x, y, entity, perso ))
        {
            delete slots[selection];
            slots[selection] = nullptr;
        }
    }
}
