#pragma once
#ifndef QUEST_GAME_GAME_HPP_INCLUDED
#define QUEST_GAME_GAME_HPP_INCLUDED

#include <memory>

#include "../graphics/Gui.hpp"

#include "Actor.hpp"
#include "Map.hpp"
#include "Cheats.hpp"
#include "Roll.hpp"

namespace Game
{

struct Game
{
   using ActorVec = std::vector<Actor::SmartPtr>;

   Player   player;
   ActorVec actors;
   Map      map;
   Cheats   cheats;

   bool game_ended = false;
   
   //! Create actor
   int createActor(Actor::Type type, int x, int y)
   {
      for(decltype(actors.size()) i = 0; i < actors.size(); ++i)
      {
         if(!actors[i])
         {
            actors[i] = Actor::create(type, x, y);
            actors[i]->index = i;
            return i;
         }
      }

      return -1;
   }

   int createNpc(const std::string& type, int x, int y)
   {
      for(decltype(actors.size()) i = 0; i < actors.size(); ++i)
      {
         if(!actors[i])
         {
            actors[i] = Actor::createActor(type, x, y);
            actors[i]->index = i;
            return i;
         }
      }

      return -1;
   }
   
   //! Get actor
   Actor* getActor(int actor_index)
   {
      if(actor_index >= 0 && actor_index < int(actors.size()))
      {
         return actors[actor_index].get();
      }
      return nullptr;
   }

   //!
   void removeActor(int actor_index)
   {
      if(actor_index >= 0 && actor_index < int(actors.size()))
      {
         actors[actor_index] = Actor::SmartPtr{ nullptr };
      }
   }

   //! Draw actors
   void drawActors()
   {
      player.draw(map);
      for(auto& actor : actors)
      {
         if(actor)
         {
            actor->draw(map);
         }
      }
   }

   void drawStats() const
   {
      player.drawStats();
   }

   void drawMap() const
   {
      map.draw();
   }

   bool checkInteraction(Actor& actor, int x, int y)
   {
      for(auto& other : actors)
      {
         if(other)
         {
            if(other->x == x && other->y == y)
            {
               actor.interact(*other);
               return true;
            }
         }
      }
      return false;
   }

   void gameOver()
   {
      this->game_ended = true;
   }

   bool ended() const
   {
      return this->game_ended;
   }
};

inline std::unique_ptr<Game> instance = std::unique_ptr<Game>{ nullptr };

inline bool checkMove(const Actor& actor, int x, int y)
{
   return instance->map.isMoveOkay(actor, y, x);
}

inline void performMove(Actor& actor, int x, int y)
{
   instance->map.moveOff(actor);
   actor.set_xy(x, y);
   instance->map.moveOn(actor);
}

inline void initialize()
{
   seedRolls();

   instance = std::unique_ptr<Game>{ new Game{} };
   instance->player.x = 10;
   instance->player.y = 6;
   
   instance->actors.resize(10);
   
   instance->map = Map::load("default.map");

   Engine::Keyboard& kb = Engine::Keyboard::instance();
   auto& player   = instance->player;
   auto& game_map = instance->map;

   auto move_up = [&player, &game_map](const char&) {
      if (!player.interacting && !instance->checkInteraction(player, player.x, player.y - 1))
      {
         if (game_map.isMoveOkay(player, player.y - 1, player.x))
         {
		      performMove(player, player.x, player.y - 1);
         }
      }
   };

   auto move_down = [&player, &game_map](const char&) {
      if (!player.interacting && !instance->checkInteraction(player, player.x, player.y + 1))
      {
	      if (game_map.isMoveOkay(player, player.y + 1, player.x)) 
         {
		      performMove(player, player.x, player.y + 1);
	      }
      }
   };

   auto move_left = [&player, &game_map](const char&) {
      if (!player.interacting && !instance->checkInteraction(player, player.x - 1, player.y))
      {
	      if (game_map.isMoveOkay(player, player.y, player.x - 1)) 
         {
		      performMove(player, player.x - 1, player.y);
	      }
      }
   };

   auto move_right = [&player, &game_map](const char&) {
      if (!player.interacting && !instance->checkInteraction(player, player.x + 1, player.y))
      {
	      if (game_map.isMoveOkay(player, player.y, player.x + 1)) 
         {
		      performMove(player, player.x + 1, player.y);
	      }
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
