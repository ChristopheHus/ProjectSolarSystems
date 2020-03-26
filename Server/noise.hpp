#ifndef NOISE_INCLUDED
#define NOISE_INCLUDED

#include "defs.hpp"

class PerlinNoise
{
public:
  PerlinNoise();
  ~PerlinNoise();
  /**
  * Génère un bruit de Perlin
  * terrain[] : le tableau dans lequel seront sockées les valeurs
  * s : la taille du tableau
  * height : la différence entre la valeur maximum et minimum
  * offset : la valeur minimum
  **/
  void genTerrain( int terrain[], int const &s, float const &height, float const &offset, int const &nbOctaves, int const &maxOSize, int const &interpolation );
private:
  /**
  * Génère un octave du bruit de Perlin
  * terrain[] : le tableau dans lequel seront sockées les valeurs
  * s : la taille du tableau
  * octave : le numéro de l'octave calculé
  **/
  void genOctave( float terrain[], int const &s, int const &octave, float values[], int const &maxOSize, int const &valuesStart, int const &interpolation );
  /**
  * Génère une partie d'un octave
  * terrain[] : le tableau dans lequel seront sockées les valeurs
  * [ xmin, xmax [ : l'intervalle dans lequel on doit remplir le tableau
  * y1, y2 : valeur de début et de fin de la vague
  **/
  void genWave( float terrain[], int const &xmin, int const &xmax, float const &y1, float const &y2, int const &interpolation );
  /**
  * Renvoi le résultat d'une interpolation
  * x1, y1 : les coordonnées du premier point de la courbe
  * x2, y2 : les coordonnées du deuxième point de la courbe
  * x : l'abscisse du point à calculer
  **/
  float interpolation0( float const &x1, float const &y1, float const &x2, float const &y2, float const &x );
  float interpolation1( float const &x1, float const &y1, float const &x2, float const &y2, float const &x );
  float interpolation2( float const &x1, float const &y1, float const &x2, float const &y2, float const &x );
};

#endif // NOISE_INCLUDED
