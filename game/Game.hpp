#pragma once
#ifndef QUEST_GAME_GAME_HPP_INCLUDED
#define QUEST_GAME_GAME_HPP_INCLUDED

#include <memory>

#include "../graphics/Gui.hpp"

#include "Actor.hpp"
#include "Map.hpp"
#include "Cheats.hpp"

namespace Game
{

struct Game
{
   Player player;
   Npc    npc;
   Map    map;
   Cheats cheats;

   void drawPlayer() const
   {
      npc   .draw(map.m_map_window->win);
      player.draw(map.m_map_window->win);
   }

   void drawMap() const
   {
      map.draw();
   }

   bool checkInteraction(int x, int y)
   {
      if(npc.x == x && npc.y == y)
      {
         Graphics::Gui::instance->message("Interaction!\n");
         player.interact(npc);
         return true;
      }
      return false;
   }

   void refresh()
   {
      map.refresh();
   }
};

inline std::unique_ptr<Game> instance = std::unique_ptr<Game>{ nullptr };

inline void initialize()
{
   instance = std::unique_ptr<Game>{ new Game{} };
   
   instance->map = Map::load("binary.map");
   instance->npc.set_xy(2, 2);

   Engine::Keyboard& kb = Engine::Keyboard::instance();
   auto& player   = instance->player;
   auto& game_map = instance->map;

   auto move_up = [&player, &game_map](const char&) {
      if (!player.interacting && !instance->checkInteraction(player.x, player.y - 1))
      {
         if (game_map.isMoveOkay(player, player.y - 1, player.x))
         {
            player.y = player.y - 1;
         }
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
   //
   
   instance->cheats.addKeyboardCombo(std::vector<char>{'i', 'd', 'd', 'q', 'd'}, [](){
      Graphics::Gui::instance->message("LOL DOOM\n");
   });
   instance->cheats.addKeyboardCombo(std::vector<char>{'i', 'd', 'c', 'l', 'i', 'p'}, [&player](){
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
   });
   instance->cheats.enable();
}

} /* namespace Game */

#endif /* QUEST_GAME_GAME_HPP_INCLUDED */
