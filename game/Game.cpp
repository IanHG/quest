#include "Game.hpp"

namespace Game
{

/**
 * Move an actor.
 *
 * @param x
 * @param y
 * @param relative   Move relative to current position
 **/
void moveCharacter(Actor& actor, int x, int y, bool relative)
{
   //auto  map_index = actor.map_index;
   auto& game_map  = instance->map;
   
   x = relative ? actor.x + x : x;
   y = relative ? actor.y + y : y;

   auto performMove = [&game_map](Actor& actor, int x, int y)
   {
      game_map.moveOff(actor);
      actor.set_xy(x, y);
      game_map.moveOn(actor);
   };

   if (game_map.isMoveOkay(actor, y, x))
   {
      performMove(actor, x, y);
   }
   else if(game_map.isMapExit(actor, x, y))
   {
      // FIX
      handleMapExit(WorldMap::Exit::North);
   }
   if(game_map.isSpecialMapExit(actor, x, y))
   {
      // FIX
      handleMapExit(WorldMap::Exit::Special);
   }
}

} /* namespace Game */
