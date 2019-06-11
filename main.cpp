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

   /* initialize curses */
   Graphics::initialize();
   Engine  ::initialize();

   /* initialize the quest map */
   Game::Player player;
   player.set_xy(0, 6);

   Game::Map game_map = Game::Map::load("binary.map");
   
   Engine::Keyboard& kb = Engine::Keyboard::instance();
   auto move_up = [&player, &game_map](const char&) {
      if (game_map.isMoveOkay(player, player.y - 1, player.x))
      {
         player.y = player.y - 1;
      }
   };

   auto move_down = [&player, &game_map](const char&) {
	   if (game_map.isMoveOkay(player, player.y + 1, player.x)) 
      {
		   player.y = player.y + 1;
	   }
   };

   auto move_left = [&player, &game_map](const char&) {
	   if (game_map.isMoveOkay(player, player.y, player.x - 1)) 
      {
	      player.x = player.x - 1;
	   }
   };

   auto move_right = [&player, &game_map](const char&) {
	   if (game_map.isMoveOkay(player, player.y, player.x + 1)) 
      {
		   player.x = player.x + 1;
	   }
   };

   struct exit_exception
   {
   };

   auto quit = [](const char&)
   {
      throw exit_exception{};
   };

   kb.registerEvent('w',    move_up);
   kb.registerEvent('W',    move_up);
   //kb.registerEvent(KEY_UP, move_up);
   kb.registerEvent('s',      move_down);
   kb.registerEvent('S',      move_down);
   //kb.registerEvent(KEY_DOWN, move_down);
   kb.registerEvent('a',      move_left);
   kb.registerEvent('A',      move_left);
   //kb.registerEvent(KEY_LEFT, move_left);
   kb.registerEvent('d',       move_right);
   kb.registerEvent('D',       move_right);
   //kb.register_event(KEY_RIGHT, move_right);
   //kb.register_event('q', quit);
   //kb.register_event('Q', quit);
   //keyboard_queue& queue = keyboard_queue::instance();
   //
   Engine::KeyboardCombo kbc{std::vector<char>{'i', 'd', 'd', 'q', 'd'}, [](){
      Graphics::Gui::instance->message("LOL DOOM\n");
   }, kb};
   Engine::KeyboardCombo kbc2{std::vector<char>{'i', 'd', 'c', 'l', 'i', 'p'}, [&player](){
      if(!player.noclip)
      {
         Graphics::Gui::instance->message("Nothing can stop me!\n");
         player.noclip = true;
      }
      else
      {
         Graphics::Gui::instance->message("I'm bound by reality!\n");
         player.noclip = false;
      }
   }, kb};

   kbc.enable();
   kbc2.enable();
   
   try
   {
      /******************************************
       * MAIN GAME LOOP
       ******************************************/
      Engine::gameLoop([&game_map, &player](){ 
         game_map.draw();
         player.draw(game_map.m_map_window->win);
         game_map.refresh();

         return true; 
      });
   }
   catch(const exit_exception& e)
   {

   }
   
   Graphics::finalize();
}
