#include "texture.hpp"

/*********************************************
**
** Class Image
**
*********************************************/

/** Erreurs :
*** -1 : Texture non definie
*** 1 : surface non load
*** 2 : texture non cree
**/

/// Constructeur pour toute l'image
Image::Image( std::string path ){
    error = 0;

    SDL_Surface* image;
    image = IMG_Load(path.c_str());

    if( !image ){ error = 1; return; }

    width = image->w;
    height = image->h;
    texture = SDL_CreateTextureFromSurface( getrenderer(), image );

    if( !texture ){ error = 2; SDL_FreeSurface( image ); return; }

    src = { 0, 0, image->w, image->h };

    SDL_FreeSurface( image );
}

/// Constructeur pour une partie de l'image
Image::Image( std::string path , const int x, const int y, const int w, const int h ){
    error = 0;

    SDL_Surface* image;
    image = IMG_Load(path.c_str());

    if( !image ){ error = 1; return; }

    width = w;
    height = h;
    texture = SDL_CreateTextureFromSurface( getrenderer(), image );

    if( !texture ){ error = 2; SDL_FreeSurface( image ); return; }

    src = { x, y, w, h };

    SDL_FreeSurface( image );
}

/// Destructeur de la class Image
Image::~Image( ){
    if( error == 0 )
        SDL_DestroyTexture( texture );
}

/// Rendre la texture aux coordonnés donnés
int Image::render( const int x, const int y ){
    return render( x, y, 1, 1 );
}

/// Rendre la texture aux coordonnés donnés, à l'échelle donnée
int Image::render( const int x, const int y, const double multipX, const double multipY ){
    if( error ){ return error; }

    SDL_Rect dst = { x, y, int(double(width)*multipX), int(double(height)*multipY) };

    SDL_RenderCopy( getrenderer(), texture, &src, &dst );

    return 0;
}

/// Changer le SDL_Rect source
int Image::setRectSrc( const int x, const int y, const int w, const int h ){
    src = { x, y, w, h };
    return 0;
}


/*********************************************
**
** Class MultiImage
**
*********************************************/

MultiImage::MultiImage( std::string path, int nbX, int nbY ){
    error = 0;

    SDL_Surface* image;
    image = IMG_Load(path.c_str());

    if( !image ){ error = 1; return; }

    width = image->w / nbX ;
    height = image->h / nbY ;
    texture = SDL_CreateTextureFromSurface( getrenderer(), image );

    if( !texture ){ error = 2; SDL_FreeSurface( image ); return; }

    SDL_FreeSurface( image );
}

MultiImage::~MultiImage(){
    SDL_DestroyTexture( texture );
}

int MultiImage::render( int const x, int const y, int const nImageX, int const nImageY ){
    if( error ){ return error; }

    SDL_Rect dst = { x, y, width, height };
    SDL_Rect src = { nImageX * width, nImageY * height, width, height };

    SDL_RenderCopy( getrenderer(), texture, &src, &dst );

    return 0;
}

int MultiImage::render( int const x, int const y, double const multipX, double const multipY, int const nImageX, int const nImageY ){
    if( error ){ return error; }

    SDL_Rect dst = { x, y, int(double(width)*multipX), int(double(height)*multipY) };
    SDL_Rect src = { nImageX * width, nImageY * height, width, height };

    SDL_RenderCopy( getrenderer(), texture, &src, &dst );

    return 0;
}


/*********************************************
**
** Class Anim
**
*********************************************/

/// Constructeur de la class Anim
Anim::Anim( std::string path, int images[][4], const int l, const double mPI ){
    msPerImg = mPI;
    fLength = l;
    if( fLength > 8 ) fLength = 8;
    nbMs = clock();
    for( int i=0; i<fLength; i++ )
        frames.push_back( Image( path, images[i][0], images[i][1], images[i][2], images[i][3] ) );
    nImg = 0;
}

/// Destructeur de la class Anim
Anim::~Anim( ){
    //delete frames;
}

/// On rend l'img en cours
int Anim::render( const int x, const int y, const double multipX, const double multipY ){
    frames[nImg].render( x, y, multipX, multipY );
    return 0;
}

/// On calcul le temps passé et on change l'img en cours si besoin
int Anim::update(){
    if( (double)(clock() - nbMs) > msPerImg ){
        nbMs = clock();
        nImg ++;
        if( nImg > fLength )
            nImg = 0;
    }
    return 0;
}
