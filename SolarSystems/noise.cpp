#include "noise.hpp"

/*********************************************
**
** Class PerlinNoise
**
*********************************************/

PerlinNoise::PerlinNoise(){
}

void PerlinNoise::genTerrain( int terrain[], float height, float offset, int num, int chunkwidth ){
    chunkWidth = chunkwidth;

    //Intialisation des tableaux
    float noise[chunkWidth];
    float n[chunkWidth];
    for(int i=0; i<chunkWidth; i++){
        noise[i] = 0;
        n[i] = 0;
    }

    //Generation des octaves
    for(int i=0; i<4; i++){
        genOctave( n, i, num );
        for(int j=0; j<chunkWidth; j++){
            noise[j] += n[j];
        }
    }

    //Multiplication du bruit par la hauteur et ajout de la hauteur minimale
    for(int i=0; i<chunkWidth; i++)
        terrain[i] = (int)(noise[i]*height + offset + .5);
}

void PerlinNoise::genOctave( float terrain[], int octave, int n ){
    float amplitude ( pow( .25, octave ) );
    int frequency ( chunkWidth/pow( 2, octave ));
    float height;
    if(n==0) height = genSmoothNoise( 256*chunkWidth-1, frequency );
    else height = genSmoothNoise( n*chunkWidth-1, frequency );

    for( int i=frequency-1; i<chunkWidth; i+= frequency ){

        float random = genSmoothNoise( n*chunkWidth + i, frequency );
        for(int j=i-frequency+1; j<=i; j++)
        {
            terrain[j] = interpolation( (float)(i-frequency), height, (float)i, random, (float)j ) * amplitude;
        }
        height = random;
    }
}

float PerlinNoise::interpolation( float x1, float y1, float x2, float y2, float x ){
    float xx = (float)(x-x1) / (float)(x2-x1);
    return (y2-y1)*( 3*xx*xx - 2*xx*xx*xx ) + y1;
}

float PerlinNoise::genSmoothNoise( int x, int frequency ){
    return genWhiteNoise(x);//2 + genWhiteNoise(x-frequency)/4 + genWhiteNoise(x+frequency)/4;
}

float PerlinNoise::genWhiteNoise( int x ){
    //long xx = (x<<13) ^ x;
    return (float)((x*197+4721)%1000+1)/1000.0;//( 1.0 - (float)( (xx * (xx * xx * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}
