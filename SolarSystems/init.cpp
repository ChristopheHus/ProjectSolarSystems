#include "init.hpp"

SDL_Window *screen, *console;
SDL_Renderer *renderer, *renConsole;
TTF_Font *smallFont;
TTF_Font *bigFont;
map<string, SDL_Color> colors;
SDL_Surface *txtSurface;
SDL_Texture *txtTexture;
SDL_Rect dst;
string consoleLines[18];
bool useConsole = true;

const string version = "0.4.9";

SDL_Renderer *getrenderer(){
    return renderer;
}

SDL_Window *getscreen(){
    return screen;
}

void init(char* title){
    if( SDL_Init(SDL_INIT_VIDEO) ){cout << "SDL non initialisee" << endl << SDL_GetError() << endl; exit(-1);}

    //Creation de l'ecran et du renderer
    console = SDL_CreateWindow( "Console", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 630, 378, SDL_WINDOW_SHOWN );
    screen = SDL_CreateWindow( title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_FOREIGN );
    if( !screen || !console ){cout << "Ecrans non construit" << endl << SDL_GetError() << endl; exit(1);}

    renConsole = SDL_CreateRenderer( console, -1, NULL );
    renderer = SDL_CreateRenderer( screen, -1, SDL_RENDERER_PRESENTVSYNC );
    if( !renderer || !renConsole ){cout << "Renderer non construit" << endl << SDL_GetError() << endl; exit(1);}

    for(int i=0; i<18; i++)
        consoleLines[i] = " ";
    SDL_SetRenderDrawColor( renConsole, 150, 150, 150, 255 );
    SDL_RenderClear( renConsole );
    SDL_RenderPresent( renConsole );

    //Initialisation sdl_image
    int imgFlags = IMG_INIT_PNG;
    if( IMG_Init( imgFlags ) != imgFlags )
    {
        cout << "SDL_image n'a pu être initialisée! SDL_image Error: " << IMG_GetError() << endl;
        exit(1);
    }

    ///SDL_ShowCursor(SDL_DISABLE); si on change l'img du cursor

    //Initialisation sdl_ttf -> texte
    if (TTF_Init() < 0)
    {
        cout << "Impossible d'initialiser SDL_TTF: " << TTF_GetError() << endl;
        exit(1);
    }

    //Initialisation sdl_mixer -> son
    int flags = MIX_INIT_MP3;
    int initted = Mix_Init(flags);
    if ((initted & flags) != flags)
    {
        cout << "Mix_Init: Failed to init SDL_Mixer" << endl;
        cout << "Mix_Init: " << Mix_GetError() << endl;
        exit(1);
    }

    /* Open 44.1KHz, signed 16bit, system byte order,
    stereo audio, using 1024 byte chunks (voir la doc pour plus d'infos) */
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        cout << "Mix_OpenAudio: " << Mix_GetError() << endl;
        exit(1);
    }

    // Définit le nombre de pistes audio (channels) à mixer
    Mix_AllocateChannels(32);

    bigFont = TTF_OpenFont( "FFont.ttf", 70 );
    smallFont = TTF_OpenFont( "FFont.ttf", 35 );

    if( bigFont==nullptr || smallFont==nullptr )
    {
        cout << "Fonts non initialises " << TTF_GetError() << endl;
        exit(1);
    }

    colors["black"] = { 25, 22, 22 };
    colors["white"] = { 221, 221, 221 };
    colors["grey"] = { 64, 64, 64 };
    colors["silver"] = { 154, 161, 161 };
    colors["brown"] = { 79, 50, 31 };
    colors["red"] = { 150, 52, 48 };
    colors["orange"] = { 219, 125, 62 };
    colors["yellow"] = { 188, 177, 39 };
    colors["lime"] = { 65, 174, 56 };
    colors["green"] = { 53, 70, 27 };
    colors["cyan"] = { 46, 110, 137 };
    colors["lightBlue"] = { 107, 138, 201 };
    colors["blue"] = { 46, 56, 141 };
    colors["purple"] = { 126, 61, 181 };
    colors["magenta"] = { 179, 80, 188 };
    colors["pink"] = { 208, 132, 153 };

    write( "Initialisation ... ok" );
}

void quit(){
    //Fermeture de sdl_mixer
    Mix_CloseAudio();
    Mix_Quit();

    //Destruction des variables pointees
    closeConsole();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    TTF_CloseFont( bigFont );
    bigFont = NULL;
    TTF_CloseFont( smallFont );
    smallFont = NULL;

    //Fermeture de sdl_ttf
    TTF_Quit();

    //Fermeture de la sdl
    SDL_Quit();

}

TTF_Font *getSmallFont()
{
    return smallFont;
}

TTF_Font *getBigFont()
{
    return bigFont;
}

void drawText( string text, string color, bool fontSize, SDL_Texture *texture, int x, int y )
{
    dst.x = x;
    dst.y = y;
    if(fontSize)
        txtSurface = TTF_RenderText_Solid( getBigFont(), text.c_str(), colors[color] );
    else
        txtSurface = TTF_RenderText_Solid( getSmallFont(), text.c_str(), colors[color] );

    txtTexture = SDL_CreateTextureFromSurface( getrenderer(), txtSurface );
    dst.w = txtSurface->w;
    dst.h = txtSurface->h;
    if(texture!=NULL)
        SDL_SetRenderTarget( getrenderer(), texture );
    SDL_RenderCopy( getrenderer(), txtTexture, NULL, &dst );
    SDL_FreeSurface(txtSurface);
    SDL_DestroyTexture(txtTexture);
}

string to_string( int arg )
{
    stringstream strs;
    strs << arg;
    return strs.str();
}

void write( string const &text )
{
    if(!useConsole) return;
    SDL_RenderClear( renConsole );

    for(int i=16; i>=0; i--)
        consoleLines[i+1] = consoleLines[i];
    consoleLines[0] = ": " + text;

    for(int i=0; i<18; i++)
    {
        dst.x = 0;
        dst.y = (17-i)*21;//char size : 19 pixels

        txtSurface = TTF_RenderText_Solid( getSmallFont(), consoleLines[i].c_str(), colors["white"] );
        txtTexture = SDL_CreateTextureFromSurface( renConsole, txtSurface );
        dst.w = txtSurface->w;
        dst.h = txtSurface->h;

        SDL_RenderCopy( renConsole, txtTexture, NULL, &dst );
        SDL_FreeSurface(txtSurface);
        SDL_DestroyTexture(txtTexture);
    }
    SDL_RenderPresent( renConsole );
}

void closeConsole()
{
    if(!useConsole) return;
    SDL_DestroyRenderer(renConsole);
    SDL_DestroyWindow(console);
    console = nullptr;
    renConsole = nullptr;
    useConsole = false;
}
