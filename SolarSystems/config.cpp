#include "config.hpp"

/*********************************************
**
** Class Config
**
*********************************************/

Config::Config() : buttons(768,512)
{
    game = SDL_CreateTexture( getrenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT );
    dst = { 0, 0, 0, 0 };
}

Config::~Config()
{

}

void Config::initialize()
{
    buttons.clearArea();
    buttons.newButton(1,100,100,60,30);
    buttons.newButton(2,100,150,120,30);
    buttons.newButton(3,100,200,120,30);
    buttons.newButton(4,100,250,150,30);
    buttons.newButton(5,100,300,300,30);
}

void Config::render()
{
    SDL_SetRenderTarget( getrenderer(), game );

    //Render sky
    SDL_SetRenderDrawColor( getrenderer(), 1, 2, 19, 255 );//135 206 235
    SDL_RenderClear( getrenderer() );

    drawText( "Config", "white", true, NULL, 100, 50 );

    drawText( "Up", "white", false, NULL, 100, 100 );
    drawText( "Down", "white", false, NULL, 100, 150 );
    drawText( "Left", "white", false, NULL, 100, 200 );
    drawText( "Right", "white", false, NULL, 100, 250 );
    drawText( "Inventory", "white", false, NULL, 100, 300 );

    drawText( to_string(keyUp), "white", false, NULL, 500, 100 );
    drawText( to_string(keyDown), "white", false, NULL, 500, 150 );
    drawText( to_string(keyLeft), "white", false, NULL, 500, 200 );
    drawText( to_string(keyRight), "white", false, NULL, 500, 250 );
    drawText( to_string(keyInventory), "white", false, NULL, 500, 300 );

    SDL_SetRenderTarget( getrenderer(), NULL );
    SDL_GetWindowSize( getscreen(), &dst.w, &dst.h );
    SDL_RenderCopy( getrenderer(), game, NULL, &dst );

    SDL_RenderPresent( getrenderer() );
}

void Config::update()
{
    manageEvent( ((float)SCREEN_WIDTH/dst.w), 0, ((float)SCREEN_HEIGHT/dst.h), 0 );
    if(key[EscKey]) changeState( "menu" );
    if(mButtonPressed[0])
    {
        switch(buttons.clickAt(mousePos[0],mousePos[1]))
        {
        case 1: keyUp = getKeyPressed();
            break;
        case 2: keyDown = getKeyPressed();
            break;
        case 3: keyLeft = getKeyPressed();
            break;
        case 4: keyRight = getKeyPressed();
            break;
        case 5: keyInventory = getKeyPressed();
            break;
        }
    }
}

void Config::leave()
{

}

int Config::getKeyPressed()
{
    int key(0);
    bool stop(false);
    SDL_Rect rect = { 320, 224, 128, 64 };
    SDL_SetRenderTarget( getrenderer(), game );

    SDL_SetRenderDrawColor( getrenderer(), 50, 50, 50, 255 );
    SDL_RenderFillRect( getrenderer(), &rect );

    SDL_SetRenderTarget( getrenderer(), NULL );
    SDL_GetWindowSize( getscreen(), &dst.w, &dst.h );
    SDL_RenderCopy( getrenderer(), game, NULL, &dst );

    SDL_RenderPresent( getrenderer() );

    while(!stop)
    {
        SDL_WaitEvent( &event );
        switch(event.type){
        case SDL_QUIT:
            exit(); stop = true;
            break;
        case SDL_WINDOWEVENT:
            if(event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(getscreen()))
                exit(); stop = true;
            break;
        case SDL_KEYDOWN:
            key = event.key.keysym.sym;
            if( key!=SDLK_ESCAPE ) stop = true;
        }
    }
    return key;
}
