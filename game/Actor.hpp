#pragma once
#ifndef QUEST_GAME_ACTOR_HPP_INCLUDED
#define QUEST_GAME_ACTOR_HPP_INCLUDED

#include "../graphics/TypeDefs.hpp"
#include "../graphics/Sprite.hpp"

namespace Game
{

struct Actor
{
   //
   enum : int { error, player, npc };

   // Sprite
   Graphics::SpriteProxy sprite = Graphics::SpriteContainer::instance->getSprite(' ');

   // Position
   int  x = 0;
   int  y = 0;
   
   // 
   bool passable = false;
   bool flying   = false;
   bool noclip   = false;
   bool godmode  = false;
   
   // 
   int type = error;

   // virtual destructor
   virtual ~Actor() = 0;
   
   // Draw actor
   void draw(WINDOW* win)
   {
      this->sprite->draw(win, x, y);
   }
   
   // Interact with other actor
   virtual void interact(Actor& other)
   {
   }
   
   // 
   virtual void onMoveOver(Actor& other)
   {
   }
};

inline Actor::~Actor() 
{ 
}

struct Character
   :  public Actor
{
   // Virtual destructor
   virtual ~Character() = 0;


   // Stats
   int hp;
};

inline Character::~Character()
{
}

struct Npc
   :  public Character
{
   bool      hostile   = false;
   //Encounter encounter; 

   Npc()
   {
      Actor    ::type = Actor::npc;
      Character::hp   = 10;
   }

   void interact(Actor& other)
   {
      if(other.type == Actor::player)
      {
         if(hostile)
         {
            // Attack
         }
         else
         {
            // Conversation
            //this->encounter.start(other);
         }
      }
   }
};

struct Player
   :  public Character
{
   Player()
   {
      Actor    ::type = Actor::player;
      Character::hp   = 10;
   }
   
   void interact(Actor& other)
   {
      Npc* npc = dynamic_cast<Npc*>(&other);
      if(npc)
      {
         if(npc->hostile)
         {
            // Attack
         }
         else
         {
            // Conversation
            //npc->encounter.start(*this);
         }
      }
   }
};


//struct Item
//   :  public Actor
//{
//   std::string on_pickup = std::string{};
//};

} /* namespace engine */

#endif /* QUEST_GAME_ACTOR_HPP_INCLUDED */
