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
   //std::string braille_msg = 
   // " ⠁⠂⠃⠄⠅⠆⠇⠈⠉⠊⠋⠌⠍⠎⠏\n"
   // "⠐⠑⠒⠓⠔⠕⠖⠗⠘⠙⠚⠛⠜⠝⠞⠟\n"
   // "⠠⠡⠢⠣⠤⠥⠦⠧⠨⠩⠪⠫⠬⠭⠮⠯\n"
   // "⠰⠱⠲⠳⠴⠵⠶⠷⠸⠹⠺⠻⠼⠽⠾⠿\n";

   /******************************************
    * Initialize game components 
    ******************************************/
   Graphics::initialize();
   Engine  ::initialize();
   Game    ::initialize();
   
   /******************************************
    * MAIN GAME LOOP
    ******************************************/
   Engine::gameLoop([](){ 
      auto& game = *Game::instance;
      game.drawMap();
      game.drawPlayer();
      game.refresh();

      //game.checkInteraction();

      return true; 
   });
   
   /******************************************
    * Finalize game components 
    ******************************************/
   Graphics::finalize();
}
