/* quest.c */

#include <curses.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <sstream>
#include <fstream>

#include "graphics/Graphics.hpp"
#include "graphics/Gui.hpp"
#include "engine/Engine.hpp"
#include "engine/Keyboard.hpp"
#include "game/Actor.hpp"
#include "game/Map.hpp"
#include "game/Game.hpp"


/**
 * Main function
 **/
int main(int argc, char* argv[])
{
   
   /******************************************
    * Initialize game components 
    ******************************************/
   Graphics::initialize();
   Engine  ::initialize();
   Game    ::initialize();
   
   /******************************************
    * Start screen
    ******************************************/
   
   /******************************************
    * MAIN GAME LOOP
    ******************************************/
   Engine::gameLoop([](){ 
      auto& game = *Game::instance;
      game.drawMap();
      game.drawActors();
      game.drawStats();

      return !game.ended(); 
   });

   /******************************************
    * End screen
    ******************************************/

   /******************************************
    * Finalize game components 
    ******************************************/
   Graphics::finalize();
}
