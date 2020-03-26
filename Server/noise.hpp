#ifndef NOISE_INCLUDED
#define NOISE_INCLUDED

#include "defs.hpp"

class PerlinNoise
{
public:
  PerlinNoise();
  ~PerlinNoise();
  /**
  * G�n�re un bruit de Perlin
  * terrain[] : le tableau dans lequel seront sock�es les valeurs
  * s : la taille du tableau
  * height : la diff�rence entre la valeur maximum et minimum
  * offset : la valeur minimum
  **/
  void genTerrain( int terrain[], int const &s, float const &height, float const &offset, int const &nbOctaves, int const &maxOSize, int const &interpolation );
private:
  /**
  * G�n�re un octave du bruit de Perlin
  * terrain[] : le tableau dans lequel seront sock�es les valeurs
  * s : la taille du tableau
  * octave : le num�ro de l'octave calcul�
  **/
  void genOctave( float terrain[], int const &s, int const &octave, float values[], int const &maxOSize, int const &valuesStart, int const &interpolation );
  /**
  * G�n�re une partie d'un octave
  * terrain[] : le tableau dans lequel seront sock�es les valeurs
  * [ xmin, xmax [ : l'intervalle dans lequel on doit remplir le tableau
  * y1, y2 : valeur de d�but et de fin de la vague
  **/
  void genWave( float terrain[], int const &xmin, int const &xmax, float const &y1, float const &y2, int const &interpolation );
  /**
  * Renvoi le r�sultat d'une interpolation
  * x1, y1 : les coordonn�es du premier point de la courbe
  * x2, y2 : les coordonn�es du deuxi�me point de la courbe
  * x : l'abscisse du point � calculer
  **/
  float interpolation0( float const &x1, float const &y1, float const &x2, float const &y2, float const &x );
  float interpolation1( float const &x1, float const &y1, float const &x2, float const &y2, float const &x );
  float interpolation2( float const &x1, float const &y1, float const &x2, float const &y2, float const &x );
};

#endif // NOISE_INCLUDED
