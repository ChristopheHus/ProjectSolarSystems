#ifndef NOISE
#define NOISE

#include "defs.hpp"

///Class definissant le bruit de perlin
class PerlinNoise{
public:
	PerlinNoise();
    void genTerrain( int terrain[], float height, float offset, int num, int chunkwidth );//Argument num: le numero du chunk genere
    //float getLastHeight();//avoir la hauteur maximum atteinte en ce point par le bruit
protected:
	void genRandom();//genere un tableau de valeur entre 0 et 1
    void genOctave( float terrain[], int octave, int n );
    float genSmoothNoise( int x, int frequency );
    float genWhiteNoise( int x );
    float interpolation( float x1, float y1, float x2, float y2, float x );
    int chunkWidth;
};

#endif // NOISE
