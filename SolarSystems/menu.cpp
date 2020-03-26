#include "menu.hpp"

/*********************************************
**
** Class Menu
**
*********************************************/

Menu::Menu() {
    backGround = new Image("title.png");
    buttonState = 0;
    txtClr = { 255, 255, 255 };
    activTxtClr = { 255, 255, 80 };
    dst = { 0, 0, 0, 0 };
    buffer = SDL_CreateTexture( getrenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT );
}

Menu::~Menu(){
    delete backGround;
    SDL_FreeSurface( text );
    SDL_DestroyTexture(textureText);
    SDL_DestroyTexture(buffer);
}

void Menu::initialize()
{
    Image vaisseau( "vaisseau.png" );

    for(int x=-127;x<768;x+=17)
    {
        SDL_SetRenderTarget( getrenderer(), buffer );
        SDL_RenderClear( getrenderer() );
        backGround->render( 0, 0 );
        vaisseau.render( x, 300 );
        SDL_SetRenderTarget( getrenderer(), NULL );
        SDL_GetWindowSize( getscreen(), &dst.w, &dst.h );
        SDL_RenderCopy( getrenderer(), buffer, NULL, &dst );
        SDL_RenderPresent( getrenderer() );
        SDL_Delay( 10 );
    }
}

void Menu::render(){
    SDL_SetRenderTarget( getrenderer(), buffer );
    backGround->render( 0, 0 );

    drawText( version, "white", 0, NULL, 5, 490 );

    if(buttonState == 1)
    {
        drawText( "Play", "cyan", 1, NULL, 300, 320 );
        drawText( "Config", "white", 1, NULL, 300, 400 );
    }
    else if(buttonState == 2)
    {
        drawText( "Play", "white", 1, NULL, 300, 320 );
        drawText( "Config", "cyan", 1, NULL, 300, 400 );
    }
    else
    {
        drawText( "Play", "white", 1, NULL, 300, 320 );
        drawText( "Config", "white", 1, NULL, 300, 400 );
    }

    SDL_SetRenderTarget( getrenderer(), NULL );
    SDL_GetWindowSize( getscreen(), &dst.w, &dst.h );
    SDL_RenderCopy( getrenderer(), buffer, NULL, &dst );
    SDL_RenderPresent( getrenderer() );
}

void Menu::update(){
    manageEvent( ((float)SCREEN_WIDTH/dst.w), 0, ((float)SCREEN_HEIGHT/dst.h), 0 );

    if( mousePos[0]>=300 && mousePos[0]<=430 && mousePos[1]>=320 && mousePos[1]<=350 ) buttonState = 1;
    else if( mousePos[0]>=300 && mousePos[0]<=491 && mousePos[1]>=400 && mousePos[1]<=430 ) buttonState = 2;
    else buttonState = 0;

    if(mButtonPressed[LClick])
    {
        if(buttonState==1)
        {
            write("Changing state to game");
            changeState("game");
        }
        else if(buttonState==2) changeState("config");
    }
}

