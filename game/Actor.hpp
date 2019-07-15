#pragma once
#ifndef QUEST_GAME_ACTOR_HPP_INCLUDED
#define QUEST_GAME_ACTOR_HPP_INCLUDED

#include <memory>

#include "../graphics/TypeDefs.hpp"
#include "../graphics/Sprite.hpp"

#include "Encounter.hpp"

namespace Game
{

struct Map;

struct Actor
{
   using SmartPtr = std::unique_ptr<Actor>;

   //
   enum class Type : int { Error, Player, Npc, Pushable, Item };

   // Sprite
   //Graphics::SpriteProxy sprite = Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::empty);
   Graphics::SpriteProxy sprite = Graphics::SpriteContainer::instance->getSprite(' ');

   // 
   int index = -1;

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
   void draw(Map& map) const;

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
   static SmartPtr createActor(const std::string& type, int x, int y);
   
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

struct Item
   :  public Actor
{
   using ItemIndex = int;

   ItemIndex index = -1;

   Item()
   {
      Actor    ::type   = Actor::Type::Item;
      Actor    ::sprite = Graphics::SpriteContainer::instance->getSprite('+');
   }

   virtual void interact(Actor& other);

   Actor::SmartPtr create(ItemIndex type, int x, int y)
   {
      auto item  = Actor::create(Actor::Type::Item, x, y);

      Item* item_ptr = dynamic_cast<Item*>(item.get());

      item_ptr->index = type;

      return item;
   }
};

struct Character
   :  public Actor
{
   // Virtual destructor
   virtual ~Character() = 0;

   // Stats
   int strength     = 1;
   int dexterity    = 1;
   int intelligence = 1;

   int hp           = 1;
   int hp_max       = 1;
   int mp           = 1;
   int mp_max       = 1;
};

inline Character::~Character()
{
}

struct Npc
   :  public Character
{
   //bool           hostile   = false;
   bool           hostile   = true;
   EncounterProxy encounter;

   Npc()
   {
      Actor    ::type   = Actor::Type::Npc;
      Actor    ::sprite = Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Error);
      encounter         = EncounterProxy{ std::unique_ptr<Encounter>{ new Conversation{} } };
   }

   void interact(Actor& other);
};

/**
 * Player type
 **/
struct Player
   :  public Character
{
   int level = 1;
   int xp    = 0;
   int xp_for_next_level = 1;
   int next_level        = 1;

   bool interacting = false;

   Player()
   {
      Actor    ::type   = Actor::Type::Player;
      //Actor    ::sprite = Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::PLAYER);
      Actor    ::sprite = Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Player);
      Character::hp     = 10;
   }
   
   void interact(Actor& other);

   void drawStats() const;

   void giveXp(int xp);

   void checkLevelUp();
   void checkStatus();

   void update();
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
      case Actor::Type::Item:
         actor = Actor::SmartPtr{ new Item() };
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

///**
// *
// **/
//Actor::SmartPtr Actor::createActor(const std::string& type, int x, int y)
//{
//   Actor::SmartPtr actor = Actor::SmartPtr{nullptr};
//
//   if(type == "orc")
//   {
//      actor = Actor::create(Actor::Type::Npc, x, y);
//      actor->sprite = Graphics::getSprite(Sprite::Orc);
//   }
//   else if(type == "rock")
//   {
//      actor = Actor::create(Actor::Type::Pushable, x, y);
//      actor->sprite = Graphics::getSprite(Sprite::Rock);
//   }
//   
//   return actor;
//}

//struct Item
//   :  public Actor
//{
//   std::string on_pickup = std::string{};
//};

} /* namespace game */

#endif /* QUEST_GAME_ACTOR_HPP_INCLUDED */
