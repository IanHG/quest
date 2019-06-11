/* quest.c */

#include <curses.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <sstream>
#include <fstream>

#include "keyboard.hpp"
#include "keyboard_combo.hpp"

#include "graphics/Graphics.hpp"
#include "graphics/Gui.hpp"
#include "engine/Engine.hpp"
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
   
   keyboard& kb = keyboard::instance();
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

   kb.register_event('w',    move_up);
   kb.register_event('W',    move_up);
   //kb.register_event(KEY_UP, move_up);
   kb.register_event('s',      move_down);
   kb.register_event('S',      move_down);
   //kb.register_event(KEY_DOWN, move_down);
   kb.register_event('a',      move_left);
   kb.register_event('A',      move_left);
   //kb.register_event(KEY_LEFT, move_left);
   kb.register_event('d',       move_right);
   kb.register_event('D',       move_right);
   //kb.register_event(KEY_RIGHT, move_right);
   //kb.register_event('q', quit);
   //kb.register_event('Q', quit);
   //keyboard_queue& queue = keyboard_queue::instance();
   //
   keyboard_combo kbc{std::vector<char>{'i', 'd', 'd', 'q', 'd'}, [](){
      Graphics::Gui::instance->message("LOL DOOM\n");
   }, kb};
   keyboard_combo kbc2{std::vector<char>{'i', 'd', 'c', 'l', 'i', 'p'}, [&player](){
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
   
   try
   {
      /******************************************
       * SETUP
       ******************************************/
      kb.handle_noevent();
      
      //
      auto frame      = std::chrono::milliseconds(1000 / 144);
      auto next_frame = std::chrono::system_clock::now();
      auto last_frame = next_frame - frame;

      //Graphics::Gui::instance->message(braille_msg);
      
      /******************************************
       * MAIN GAME LOOP
       ******************************************/
      while (true)
      {
         game_map.draw();
         player.draw(game_map.m_map_window->win);
         game_map.refresh();
         
         // Handle keyboard events
         kb.read_event();
         kb.handle_events();

         Graphics::Gui::instance->refresh();
         
         //std::stringstream s_str;
         //s_str << "Time: "  // just for monitoring purposes
         //      << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - last_frame).count()
         //      << "ms\n";
         //Graphics::Gui::instance->message(s_str.str());
         
         // Wait for next frame
         last_frame = next_frame;
         next_frame += frame;
         std::this_thread::sleep_until(next_frame);
      }
   }
   catch(const exit_exception& e)
   {

   }
   
   Graphics::finalize();
}
