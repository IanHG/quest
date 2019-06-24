#pragma once
#ifndef QUEST_GAME_ACTOR_HPP_INCLUDED
#define QUEST_GAME_ACTOR_HPP_INCLUDED

#include <memory>

#include "../graphics/TypeDefs.hpp"
#include "../graphics/Sprite.hpp"

#include "Encounter.hpp"

namespace Game
{

struct Actor
{
   using SmartPtr = std::unique_ptr<Actor>;

   //
   enum class Type : int { Error, Player, Npc, Pushable };

   // Sprite
   //Graphics::SpriteProxy sprite = Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::empty);
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
   Type type = Type::Error;

   // virtual destructor
   virtual ~Actor() = 0;
   
   // Draw actor
   void draw(WINDOW* win) const
   {
      this->sprite->draw(win, x, y);
   }

   void set_xy(int x, int y)
   {
      this->x = x;
      this->y = y;
   }
   
   // Interact with other actor
   virtual void interact(Actor& other)
   {
   }

   static SmartPtr create(Type type, int x, int y);
   
   //// 
   //virtual void onMoveOn(Actor& other)
   //{
   //}
   //
   //// 
   //virtual void onMoveOff(Actor& other)
   //{
   //}
};

inline Actor::~Actor() 
{ 
}

struct Pushable
   :  public Actor
{
   Pushable()
   {
      Actor    ::type   = Actor::Type::Pushable;
      Actor    ::sprite = Graphics::SpriteContainer::instance->getSprite('w');
   }

   virtual ~Pushable()
   {
   }

   virtual void interact(Actor& other);
};

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
   bool           hostile   = false;
   EncounterProxy encounter;

   Npc()
   {
      Actor    ::type = Actor::Type::Npc;
      Actor    ::sprite = Graphics::SpriteContainer::instance->getSprite('O');
      Character::hp   = 10;
      encounter       = EncounterProxy{ std::unique_ptr<Encounter>{ new Conversation{} } };
   }

   void interact(Actor& other);
};

/**
 * Player type
 **/
struct Player
   :  public Character
{
   bool interacting = false;

   Player()
   {
      Actor    ::type   = Actor::Type::Player;
      //Actor    ::sprite = Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::PLAYER);
      Actor    ::sprite = Graphics::SpriteContainer::instance->getSprite('P');
      Character::hp     = 10;
   }
   
   void interact(Actor& other);
};

/**
 * Create actor
 **/
inline Actor::SmartPtr Actor::create(Actor::Type type, int x, int y)
{
   Actor::SmartPtr actor;

   switch(type)
   {
      case Actor::Type::Player:
         actor = Actor::SmartPtr{ new Player() };
         break;
      case Actor::Type::Npc:
         actor = Actor::SmartPtr{ new Npc() };
         break;
      case Actor::Type::Pushable:
         actor = Actor::SmartPtr{ new Pushable() };
         break;
      case Actor::Type::Error:
      default:
         actor = Actor::SmartPtr{ nullptr };
   }
   
   if(actor)
   {
      actor->set_xy(x, y);
   }

   return actor;
}


//struct Item
//   :  public Actor
//{
//   std::string on_pickup = std::string{};
//};

} /* namespace engine */

#endif /* QUEST_GAME_ACTOR_HPP_INCLUDED */
