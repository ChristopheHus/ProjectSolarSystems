#include "noise.hpp"

/** Class PerlinNoise **/

PerlinNoise::PerlinNoise()
{ }

PerlinNoise::~PerlinNoise()
{ }

void PerlinNoise::genTerrain( int terrain[], int const &s, float const &height, float const &offset, int const &nbOctaves, int const &maxOSize, int const &interpolation )
{
  float tot[s], oct[s], values[(s/maxOSize)*((1<<nbOctaves)-1)];
  for(int k=0;k<s;k++)
      tot[k] = 0;
  for(int k=0;k<(s/maxOSize)*((1<<nbOctaves)-1);k++)
    values[k] = (float)(rand()%1000)/1000;

  for(int i=0;i<nbOctaves;i++)
  {
    genOctave( oct, s, i, values, maxOSize, (s/maxOSize)*((1<<i)-1), interpolation );
    for(int k=0;k<s;k++)
    {
      float w = oct[k] / (1<<(2*i));
      tot[k] += w;
    }
  }
  for(int k=0;k<s;k++)
      terrain[k] = (int)( ((tot[k]*3)/4)*height + offset + .5 );
}

void PerlinNoise::genOctave( float terrain[], int const &s, int const &octave, float values[], int const &maxOSize, int const &valuesStart, int const &interpolation )
{
  int nPoints = (1 << octave) * (s/maxOSize);
  int wavelength = s / nPoints;

  for(int i=0;i<nPoints;i++)
  {
    int x = i*wavelength;
    int k;
    if(i==0) k = valuesStart+nPoints-1;//nPoints-1 = i max
    else k = valuesStart+i-1;
    genWave( terrain, x, x+wavelength,values[ k ], values[ valuesStart+i ], interpolation );
  }
}

void PerlinNoise::genWave( float terrain[], int const &xmin, int const &xmax, float const &y1, float const &y2, int const &interpolation )
{
  switch(interpolation)
  {
  case 0:
    for(int i=xmin;i<xmax;i++) terrain[i] = interpolation0( xmin, y1, xmax, y2, i );
    break;
  case 1:
    for(int i=xmin;i<xmax;i++) terrain[i] = interpolation1( xmin, y1, xmax, y2, i );
    break;
  case 2:
    for(int i=xmin;i<xmax;i++) terrain[i] = interpolation2( xmin, y1, xmax, y2, i );
    break;
  }
}

float PerlinNoise::interpolation0( float const &x1, float const &y1, float const &x2, float const &y2, float const &x )
{
  float a = (x-x1)/(x2-x1);//a = [0,1]
  return (y2-y1)*( (3*a*a)-(2*a*a*a) ) + y1;
}

float PerlinNoise::interpolation1( float const &x1, float const &y1, float const &x2, float const &y2, float const &x )
{
  float a = (x-x1)/(x2-x1);//a = [0,1]
  return (.5*(2*a-1)*(2*a-1)*(2*a-1)+.5)*(y2-y1) + y1;
}

float PerlinNoise::interpolation2( float const &x1, float const &y1, float const &x2, float const &y2, float const &x )
{
  float a = (x-x1)/(x2-x1);//a = [0,1]
  return a*(y2-y1) + y1;
}
