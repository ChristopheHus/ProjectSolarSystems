#include "game.hpp"

/*********************************************
**
** Class Game
**
*********************************************/

Game::Game(){
    cursor = { 0, 0, 16, 16 };
    affWorld = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    dst = { 0, 0, 0, 0 };
    w = SDL_CreateTexture( getrenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH+16, SCREEN_HEIGHT+16 );
    game = SDL_CreateTexture( getrenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT );
}

Game::~Game(){
    delete world;
    delete guiWorld;
    SDL_DestroyTexture(w);
    SDL_DestroyTexture(game);
}

void Game::initialize(){
    isInInv = false;
    gui = nullptr;
    int onText(saveOrNew());

    if( onText==2 )
    {
        ifstream flux ("save.world",ios::in|ios::binary);
        if(flux)
        {
            int v(0);
            flux.read((char*)&v,4);
            if(v==4)
            {
                world = new World( flux );
                player = new Perso( flux );
                flux.close();
            }
            else
            {
                write("Save file version not supported !");
                cout << "bad version" << endl;
                exit();
            }
        }
        else { genWorld(); }
    }
    else { genWorld(); }

    guiWorld = new Image("gui_world.png");
    write( "World initialized" );
}

void Game::render(){

    //Render world and GUIs
    SDL_SetRenderTarget( getrenderer(), w );

    //Render sky
    SDL_SetRenderDrawColor( getrenderer(), 105, 165, 209, 255 );//135 206 235
    SDL_RenderClear( getrenderer() );

    world->render( (int)player->getPosX()-23, (int)player->getPosY()-14 );
    if(!isInInv)
    {
        SDL_SetRenderDrawColor( getrenderer(), 0, 0, 0, 255 );
        SDL_RenderDrawRect( getrenderer(), &cursor );
    }
    SDL_SetRenderTarget( getrenderer(), game );

    affWorld.x = (int)((player->getPosX()-(int)player->getPosX())*16);
    affWorld.y = (int)((1-player->getPosY()+(int)player->getPosY())*16);

    SDL_RenderCopy( getrenderer(), w, &affWorld, NULL );
    guiWorld->render( 0, 0 );
    player->render( 368, 224 );

    if(gui==nullptr) player->inventory.render(isInInv);
    else player->inventory.render(gui);

    drawText( to_string(player->getPosX())+" : "+to_string(player->getPosY()), "white", 0, NULL, 0, 0 );

    SDL_SetRenderTarget( getrenderer(), NULL );
    SDL_GetWindowSize( getscreen(), &dst.w, &dst.h );
    SDL_RenderCopy( getrenderer(), game, NULL, &dst );

    player->renderLife( ((float)dst.w/SCREEN_WIDTH), ((float)dst.h/SCREEN_HEIGHT) );

    SDL_RenderPresent( getrenderer() );
}

void Game::update(){
    manageEvent(((float)SCREEN_WIDTH/dst.w), affWorld.x*(!isInInv), ((float)SCREEN_HEIGHT/dst.h), affWorld.y*(!isInInv) );

    if(key[EscKey]) changeState("menu");
    else if(key[InvKey])
    {
        if(isInInv) gui = nullptr;
        isInInv = !isInInv;
    }

    if(isInInv)
    {
        if(mButtonPressed[LClick])
        {
            if(gui==nullptr)
                player->inventory.update(mousePos[0],mousePos[1]);
            else
                player->inventory.update(mousePos[0],mousePos[1],gui);
        }
    }
    else
    {
        player->inventory.changeSelection(mouseWheel);
        if(mouseButton[LClick]) player->inventory.onLeftClick( world, (mousePos[0]/16+(int)player->getPosX()-23+16384)%16384, (SCREEN_HEIGHT-mousePos[1])/16+(int)player->getPosY()-13, NULL, player );
        //if(mouseButton[MClick]) inventory[select] = world->getBlockID( (mousePos[0]/16+(int)player->getPosX()-23+256)%256, (SCREEN_HEIGHT-mousePos[1])/16+(int)player->getPosY()-13, 0 );
        if(mouseButton[RClick])
        {
            gui = world->getGuiAt( (mousePos[0]/16+(int)player->getPosX()-23+16384)%16384, (SCREEN_HEIGHT-mousePos[1])/16+(int)player->getPosY()-13 );
            if( gui != nullptr )
                isInInv = true;
            else
                player->inventory.onRightClick( world, (mousePos[0]/16+(int)player->getPosX()-23+16384)%16384, (SCREEN_HEIGHT-mousePos[1])/16+(int)player->getPosY()-13, NULL, player );
        }
        if(key[UKey])player->jump();
        if(key[LKey])player->left();
        if(key[RKey])player->right();
    }

    player->update(world);
    cursor.x = mousePos[0]/16*16;
    cursor.y = mousePos[1]/16*16;
}

void Game::leave()
{
    ofstream flux("save.world",ios::out|ios::binary);
    int v(4);
    if(flux)//&&(world!=nullptr&&player!=nullptr)
    {
        flux.write((char*)&v,4);
        world->save( flux );
        player->save( flux );
        flux.close();
    }
    delete world;
}

int Game::saveOrNew()
{
    int choix(0), onText(0), x(0), y(0);
    Image bg = Image("title.png");

    while(!choix)
    {
        //Rendu
        SDL_SetRenderTarget( getrenderer(), game );
        SDL_RenderClear( getrenderer() );

        bg.render( 0, 0 );
        drawText( version, "white", 0, NULL, 5, 490 );
        switch(onText)
        {
        case 0:
            drawText( "New World", "white", true, NULL, 300 , 320 );
            drawText( "From Save", "white", true, NULL,  300 , 400 );
            break;
        case 1:
            drawText("New World", "cyan" , true, NULL,  300 , 320);
            drawText("From Save", "white" , true, NULL,  300 , 400);
            break;
        case 2:
            drawText("New World", "white" , true, NULL,  300 , 320);
            drawText("From Save", "cyan" , true, NULL,  300 , 400);
            break;
        }

        SDL_SetRenderTarget( getrenderer(), NULL );
        SDL_GetWindowSize( getscreen(), &dst.w, &dst.h );
        SDL_RenderCopy( getrenderer(), game, NULL, &dst );

        SDL_RenderPresent( getrenderer() );

        //Update
        while(SDL_PollEvent(&event)){
            switch(event.type){
            case SDL_QUIT:
                exit();
                break;
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(getscreen()))
                    exit();
                break;
            case SDL_MOUSEMOTION:
                x = event.motion.x*((float)SCREEN_WIDTH/(float)dst.w);
                y = event.motion.y*((float)SCREEN_HEIGHT/(float)dst.h);
                if( x>=300 && x<=596 && y>=320 && y<=350) onText=1;
                else if (x>=300 && x<=611 && y>=400 && y<=430) onText=2;
                else onText=0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT && onText ) choix = 1;
                break;
            }
        }
    }
    return onText;
}

void Game::genWorld()
{
    int h = 0;
    srand(time(NULL));
    int r = rand();
    write( "Seed : "+to_string(r) );
    world = new World(r,48,32);
    world->generate();
    write( "World successfully generated" );
    h = world->getHeight(0);
    while(world->getBlock(0,h,0)->isBlockTransparent())
        h--;
    h++;
    world->setBlock( Blocks::air, 0, h, 0 ); world->setBlock( Blocks::air, 0, h+1, 0 );
    world->setBlock( Blocks::air, 0, h+2, 0 ); world->setBlock( Blocks::air, 0, h+3, 0 );
    world->setBlock( Blocks::air, 1, h, 0 ); world->setBlock( Blocks::air, 1, h+1, 0 );
    world->setBlock( Blocks::air, 1, h+2, 0 );world->setBlock( Blocks::air, 1, h+3, 0 );

    player = new Perso( 0, h );
    player->inventory.addItems( ItemStack(256,1) );

    int y = world->getHeight(3);
    world->setBlock( Blocks::chest, 3, y+1, 0 );
    world->getGuiAt(3,y+1)->addItems(ItemStack(17,10));
    world->getGuiAt(3,y+1)->addItems(ItemStack(18,10));
    world->getGuiAt(3,y+1)->addItems(ItemStack(19,10));
    world->getGuiAt(3,y+1)->addItems(ItemStack(20,10));
    world->getGuiAt(3,y+1)->addItems(ItemStack(21,10));
}
