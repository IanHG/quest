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
   
   WorldMap world_map;
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
   
   void handleMapExit(WorldMap::Exit exit);
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

inline void handleMapExit(WorldMap::Exit exit)
{
   int x_world_new = instance->world_map.x_world;
   int y_world_new = instance->world_map.y_world;

   switch(exit)
   {
      case WorldMap::Exit::North:
         instance->map.exitNorth(x_world_new, y_world_new);
         break;
      case WorldMap::Exit::South:
         instance->map.exitSouth(x_world_new, y_world_new);
         break;
      case WorldMap::Exit::East:
         instance->map.exitEast(x_world_new, y_world_new);
         break;
      case WorldMap::Exit::West:
         instance->map.exitWest(x_world_new, y_world_new);
         break;
      case WorldMap::Exit::Special:
         instance->map.exitSpecial(x_world_new, y_world_new);
         break;
      default:
         break;
   }

   
   bool changed_map = false;
   if (  (x_world_new != instance->world_map.x_world) 
      && (x_world_new >= 0) 
      && (x_world_new < instance->world_map.x_size)
      )
   {
      changed_map = true;
   }
   if (  (y_world_new != instance->world_map.y_world)
      && (y_world_new >= 0) 
      && (y_world_new < instance->world_map.y_size)
      )
   {
      changed_map = true;
   }

   if(changed_map)
   {
      auto map_name = instance->world_map.getMap(x_world_new, y_world_new);

      //if(!map_name.empty())
      {
         instance->world_map.x_world = x_world_new;
         instance->world_map.y_world = y_world_new;
         
         instance->map.destroy();
         instance->map = Map::load(map_name);

         switch(exit)
         {
            case WorldMap::Exit::North:
               instance->player.y = instance->map.y_size - 1;
               break;
            case WorldMap::Exit::South:
               instance->player.y = 0;
               break;
            case WorldMap::Exit::East:
               instance->player.x = 0;
               break;
            case WorldMap::Exit::West:
               instance->player.x = instance->map.x_size - 1;
               break;
            case WorldMap::Exit::Special:
               instance->player.x  = instance->map.x_entry;
               instance->player.y  = instance->map.y_entry;
               break;
            default:
               break;
         }
      }
   }
}

inline void initialize()
{
   seedRolls();

   instance = std::unique_ptr<Game>{ new Game{} };
   instance->player.x = 10;
   instance->player.y = 6;
   
   instance->actors.resize(10);
   
   instance->world_map = WorldMap::load("WHATEVER");
   instance->map       = Map     ::load(instance->world_map.getMap(0, 0));

   Engine::Keyboard& kb = Engine::Keyboard::instance();
   auto& player   = instance->player;
   auto& game_map = instance->map;

   auto move_up = [&player, &game_map]() {
      if (!player.interacting && !instance->checkInteraction(player, player.x, player.y - 1))
      {
         if (game_map.isMoveOkay(player, player.y - 1, player.x))
         {
		      performMove(player, player.x, player.y - 1);
         }
         else if(game_map.isMapExit(player, player.x, player.y - 1))
         {
            handleMapExit(WorldMap::Exit::North);
         }
         if(game_map.isSpecialMapExit(player, player.x, player.y))
         {
            handleMapExit(WorldMap::Exit::Special);
         }
      }
   };

   auto move_down = [&player, &game_map]() {
      if (!player.interacting && !instance->checkInteraction(player, player.x, player.y + 1))
      {
	      if (game_map.isMoveOkay(player, player.y + 1, player.x)) 
         {
		      performMove(player, player.x, player.y + 1);
	      }
         else if(game_map.isMapExit(player, player.x, player.y + 1))
         {
            handleMapExit(WorldMap::Exit::South);
         }
         
         if(game_map.isSpecialMapExit(player, player.x, player.y))
         {
            handleMapExit(WorldMap::Exit::Special);
         }
      }
   };

   auto move_left = [&player, &game_map]() {
      if (!player.interacting && !instance->checkInteraction(player, player.x - 1, player.y))
      {
	      if (game_map.isMoveOkay(player, player.y, player.x - 1)) 
         {
		      performMove(player, player.x - 1, player.y);
	      }
         else if(game_map.isMapExit(player, player.x - 1, player.y))
         {
            handleMapExit(WorldMap::Exit::West);
         }
         if(game_map.isSpecialMapExit(player, player.x, player.y))
         {
            handleMapExit(WorldMap::Exit::Special);
         }
      }
   };

   auto move_right = [&player, &game_map]() {
      if (!player.interacting && !instance->checkInteraction(player, player.x + 1, player.y))
      {
	      if (game_map.isMoveOkay(player, player.y, player.x + 1)) 
         {
		      performMove(player, player.x + 1, player.y);
	      }
         else if(game_map.isMapExit(player, player.x + 1, player.y))
         {
            handleMapExit(WorldMap::Exit::East);
         }
         if(game_map.isSpecialMapExit(player, player.x, player.y))
         {
            handleMapExit(WorldMap::Exit::Special);
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
   
   instance->cheats.addKeyboardCombo(std::vector<ichtype>{'i', 'd', 'd', 'q', 'd'}, [](){
      Graphics::Gui::instance->message("LOL DOOM\n");
   });

   instance->cheats.addKeyboardCombo(std::vector<ichtype>{'i', 'd', 'c', 'l', 'i', 'p'}, [&player](){
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
