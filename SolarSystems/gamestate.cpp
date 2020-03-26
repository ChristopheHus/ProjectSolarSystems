#include "gamestate.hpp"

/*********************************************
**
** Class GameState
**
*********************************************/

GameState::GameState(){

}

GameState::~GameState(){

}

int GameState::start( int state, int fps, vector<string> *names ){
    statesName = names;
    nextState = state;

    initialize();

    while(nextState == state){
        render();
        update();
        SDL_Delay(1000/(float)fps);
    }
    leave();
    return nextState;
}

void GameState::changeState( string stateName ){
    for(int i=0; i<statesName->size(); i++){
        if((*statesName)[i]==stateName){
            nextState = i;
            return;
        }
    }
    nextState = -1;
}

void GameState::manageEvent( float multx, float addx, float multy, float addy )
{
    mouseWheel = 0;
    mButtonPressed[0] = false;
    mButtonPressed[1] = false;
    mButtonPressed[2] = false;
    key[InvKey] = false;
    key[EscKey] = false;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            exit();
            break;
        case SDL_WINDOWEVENT:
            if(event.window.event == SDL_WINDOWEVENT_CLOSE)
                if(event.window.windowID == SDL_GetWindowID(getscreen()))
                    exit();
                else
                    closeConsole();
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE) key[EscKey] = true;
            else if(event.key.keysym.sym == keyInventory) key[InvKey] = true;
            else if(event.key.keysym.sym == keyUp) key[UKey] = true;
            else if(event.key.keysym.sym == keyDown) key[DKey] = true;
            else if(event.key.keysym.sym == keyLeft) key[LKey] = true;
            else if(event.key.keysym.sym == keyRight) key[RKey] = true;
            break;
        case SDL_KEYUP:
            if(event.key.keysym.sym == SDLK_ESCAPE) key[EscKey] = false;
            else if(event.key.keysym.sym == keyInventory) key[InvKey] = false;
            else if(event.key.keysym.sym == keyUp) key[UKey] = false;
            else if(event.key.keysym.sym == keyDown) key[DKey] = false;
            else if(event.key.keysym.sym == keyLeft) key[LKey] = false;
            else if(event.key.keysym.sym == keyRight) key[RKey] = false;
            break;
        case SDL_MOUSEWHEEL:
            mouseWheel = event.wheel.y;
            break;
        case SDL_MOUSEMOTION:
            mousePos[0] = event.motion.x*multx+addx;
            mousePos[1] = event.motion.y*multy+addy;
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch(event.button.button){
            case SDL_BUTTON_LEFT: mouseButton[LClick]=true; mButtonPressed[LClick]=true; break;
            case SDL_BUTTON_MIDDLE: mouseButton[MClick]=true; mButtonPressed[MClick]=true; break;
            case SDL_BUTTON_RIGHT: mouseButton[RClick]=true; mButtonPressed[RClick]=true; break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch(event.button.button){
            case SDL_BUTTON_LEFT: mouseButton[LClick]=false; break;
            case SDL_BUTTON_MIDDLE: mouseButton[MClick]=false; break;
            case SDL_BUTTON_RIGHT: mouseButton[RClick]=false; break;
            }
            break;
        }
    }
}

/*********************************************
**
** Class GameContainer
**
*********************************************/

GameContainer::GameContainer(){
}

GameContainer::~GameContainer(){
    for(int i=0; i<states.size(); i++)
        delete states[i];
}

void GameContainer::addState( GameState *state, string stateName ){
    states.push_back(state);
    statesName.push_back(stateName);
}

void GameContainer::start(){
    int currentState(0);

    while(currentState!=-1){
        currentState = states[currentState]->start( currentState, fps, &statesName );
    }
}
