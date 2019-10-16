#pragma once
#ifndef QUEST_GAME_FACTIONS_HPP_INCLUDED
#define QUEST_GAME_FACTIONS_HPP_INCLUDED

namespace Game
{

enum Faction : int { player, monsters, human, size };

/**
 * Faction tracker for Actors
 **/
struct FactionTracker
{
   using Reputation = int;

   Reputation reputation[Faction::size] = { 0 };

   bool isHostile(Faction faction)
   {
      return this->reputation[faction] < 5;
   }

   bool checkReputation(Faction faction, Reputation rep)
   {
      return reputation[faction] >= rep;
   }
   
   void modifyReputation(Faction faction, Reputation rep)
   {
      this->reputation[faction] += rep;
   }  
};

} /* namespace game */

#endif /* QUEST_GAME_FACTIONS_HPP_INCLUDED */
