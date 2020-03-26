#ifndef TEXTURE
#define TEXTURE

#include "init.hpp"

class Image{
public:
    Image( std::string path );
    Image( std::string path , const int x, const int y, const int w, const int h );
    ~Image( );
    int render( const int x, const int y );
    int render( const int x, const int y, const double multipX, const double multipY );
    int setRectSrc( const int x, const int y, const int w, const int h );
    int getError() { return error; }
protected:
    SDL_Texture* texture;
    SDL_Rect src;
    int width;
    int height;
    int error = -1;
};

class MultiImage {
public:
    MultiImage( std::string path, int nbX, int nbY );
    ~MultiImage();
    int render( int const x, int const y, int const nImageX, int const nImageY );
    int render( int const x, int const y, double const multipX, double const multipY, int const nImageX, int const nImageY );
protected:
    SDL_Texture* texture;
    int width;
    int height;
    int error;
};

class Anim{
public:
    Anim( std::string path, int images[][4], const int l, const double mPI );
    int render( const int x, const int y, const double multipX, const double multipY );
    int update();
    ~Anim();
private:
    double msPerImg;
    time_t nbMs;
    vector<Image> frames;
    int nImg;
    int fLength;
};


#endif // TEXTURE

