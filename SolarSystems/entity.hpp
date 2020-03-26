#ifndef ENTITY
#define ENTITY

#include "texture.hpp"
#include "world.hpp"

///Classe definissant les proprietes des entites du jeu
class Entity{
public:
    //Fonction creant une entite a un point de la carte avec une vie donnee
    Entity( int health, int x, int y );
    //Similaire a la precedente avec la possibilite de rendre l'entite immortelle
    Entity( int health, int x, int y, bool invincibility );
    //Ferme le fichier texture de l'entite
    ~Entity();
    //Fonction appliquant a l'entite des degats
    void takeDamage( Entity *aggressor, float damages );
    //Fonction permettant de savoir si l'entite est vivante
    bool isAlive() { return hp>0; }
    //Fonction affichant l'image de l'entite sur la fenetre
    void render( int x, int y );
    //Fonction appelee pour changer la position de l'entite
    void update( World *world );
    //Fonction faisant aller l'entite a gauche lors de la prochaine update
    void left() { motion[0] = true; }
    //Fonction faisant aller l'entite a droite lors de la prochaine update
    void right() { motion[1] = true; }
    //Fonction faisant sauter l'entite lors de la prochaine update
    void jump() { motion[2] = true; }
    //Fonction donnant les coordonnees X de l'entite
    float getPosX() { return (float)pos[0]/16; }
    //Fonction donnant les coordonnees Y de l'entite
    float getPosY() { return (float)pos[1]/16; }

protected:
    //Fonction testant si l'entite ne fait pas de collision sur l'axe des X
    bool motionX( World *world, int x );
    //Fonction testant si l'entite ne fait pas de collision sur l'axe des Y
    bool motionY( World *world, int y );
    Image *texture;
    int a[2], v[2], pos[2];
    int hpMax;
    float hp;
    bool isInvincible;
    bool motion[3]; //Left, Right, Up
};

#endif // ENTITY
