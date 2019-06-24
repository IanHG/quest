#pragma once
#ifndef QUEST_GAME_ENCOUNTER_HPP_INCLUDED
#define QUEST_GAME_ENCOUNTER_HPP_INCLUDED

#include <memory>


namespace Game
{

struct Player;

/**
 *
 **/
struct Encounter
{
   virtual ~Encounter();

   virtual void start(Player& player);
};

//
inline Encounter::~Encounter() 
{
}

/**
 *
 **/
struct Conversation
   :  public Encounter
{ 
   virtual void start(Player& player);
};

/**
 *
 **/
struct EncounterProxy
{
   using encounter_ptr_type = std::unique_ptr<Encounter>;

   encounter_ptr_type encounter = encounter_ptr_type{ nullptr };
   
   // Start encounter
   void start(Player& player)
   {
      if(encounter)
      {
         encounter->start(player);
      }
   }
};

}

#endif /* QUEST_GAME_ENCOUNTER_HPP_INCLUDED */
