#ifndef GAMESTATE
#define GAMESTATE

#include "world.hpp"

///Class definissant la structure des gamestate utilises
class GameState{
public:
	GameState();
	virtual ~GameState();
	int start( int state, int fps, vector<string> *names );
protected:
    void virtual initialize() { }
	void virtual render(){ }
	void virtual update(){ }
	void virtual leave(){ }
	void changeState( string stateName );
	void exit(){ nextState = -1; }
    void manageEvent( float multx, float addx, float multy, float addy );
	int nextState;
	std::vector<string> *statesName;
    int mousePos[2]={ 0, 0 };
    bool mouseButton[3]={false,false,false},key[6]={false,false,false,false,false,false},mButtonPressed[3]={false,false,false};
    enum MButton:int {LClick,MClick,RClick};
    enum Key:int {UKey,DKey,LKey,RKey,InvKey,EscKey};
    int mouseWheel;
    SDL_Event event;
};

///Class contenant tout les gamestate du jeu
class GameContainer{
public:
	GameContainer();
	~GameContainer();
    void setFps( int fps ){ GameContainer::fps = fps; }
    int getFps( ){ return fps; }
	void addState( GameState *state, string stateName );
	void start();
protected:
	std::vector<GameState*> states;
	std::vector<string> statesName;
	int fps;
};

#endif // GAMESTATE

