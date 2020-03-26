#ifndef CONTAINER
#define CONTAINER

#include "item.hpp"
#include "button.hpp"

///Classe definissant les proprietes des inventaires
class Container
{
public:
    //Fonctions initialisant et detruisant les objets stockes
    Container( int n );
    Container( ifstream &file );
    virtual ~Container();
    //Fonctions permettant d'ajouter des objets a un inventaire
    int virtual addItems( ItemStack items );
    //Fonction affichant l'inventaire
    void virtual render() = 0;
    //Fonction retournant le nombre de place qu'a l'inventaire
    int getSize() { return slots.size(); }
    //Fonction permettant de sauvegarder les objets
    void save( ofstream &file );
protected:
    vector<ItemStack*> slots;
};

class Gui : public Container
{
public:
    Gui( int n );
    Gui( ifstream &file );
    virtual ~Gui();
    ItemStack virtual *exchangeSlots( ItemStack* slot, int n );
    void virtual exchangeSlots( int n1, int n2 );
    int virtual getPointedSlot( int x, int y ) = 0;
    void virtual getRectAt( SDL_Rect *rect, int slot ) = 0;
    int getGuiType() { return guiType; }
protected:
    Button buttons;
    int guiType;
};

class Chest : public Gui{
public:
    Chest( int length );
    Chest( ifstream &file );
    virtual ~Chest();
    void virtual render();
    int virtual getPointedSlot( int x, int y );
    void virtual getRectAt( SDL_Rect *rect, int slot );
protected:
    Image *inventory;
};

///Classe definissant l'inventaire du joueur
class Inventory : public Container
{
public:
    //Fonction ouvrant le fichier de la texture de l'inventaire et initialisant la selection
    Inventory();
    Inventory( ifstream &file );
    //Fonction supprimant le contenu de l'inventaire et fermant le fichier de la texture de l'inventaire
    virtual ~Inventory();
    //Fonction permettant d'afficher l'inventaire
    void virtual render() { return Inventory::render(true); }
    void render( bool inInv );
    //Fonction permettant a l'utilisateur de gerer l'inventaire
    void update( int x, int y );
    //Fonctions similaire aux precedente avec un inventaire suplementaire
    void render( Gui *gui );
    void update( int x, int y, Gui *gui );
    //Fonction appelee pour changer la selection de la barre inferieur
    void changeSelection( int offset );
    //Fonction appelee lors d'un clic droit par l'utilisateur
    void onRightClick( World *world, int x, int y, Entity *entity, Perso *perso );
    //Fonction appelee lors d'un clic gauche par l'utilisateur
    void onLeftClick( World *world, int x, int y, Entity *entity, Perso *perso );
protected:
    Image *inventory;
    int selection, selectedStack;
    SDL_Rect select, slot, mSelect;
    SDL_Texture *buffer;
    Button inv;
};

#endif // CONTAINER
