#pragma once
#ifndef QUEST_GAME_ACTOR_HPP_INCLUDED
#define QUEST_GAME_ACTOR_HPP_INCLUDED

#include <memory>

#include "../graphics/TypeDefs.hpp"
#include "../graphics/Sprite.hpp"

#include "../engine/Engine.hpp"

#include "Encounter.hpp"
#include "Item.hpp"

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

   struct InventoryCounter
   {
      InventoryItem::Type item_index = InventoryItem::Type::None;
      int                 item_count = 0;
   };

   std::vector<InventoryCounter> inventory;

   void pickUpItem(InventoryItem::Type item_index, int item_count)
   {
      for(int i = 0; i < int(inventory.size()); ++i)
      {
         if(item_index == inventory[i].item_index)
         {
            inventory[i].item_count += item_count;
            return;
         }
      }
      
      inventory.emplace_back(InventoryCounter{item_index, item_count});
   }

   void useItem(int inventory_index, int times)
   {
      if(inventory_index >= 0 && inventory_index < int(inventory.size()))
      {
         auto& item_counter = this->inventory[inventory_index];
         auto& item         = InventoryItemContainer::getItem(item_counter.item_index);
         for(int itimes = 0; itimes < times; ++itimes)
         {
            item.on_usage(*this);
            item_counter.item_count -= 1;
            if(item_counter.item_count <= 0)
            {
               break;
            }
         }
      }
   }

   void incrementHp(int hp_incr)
   {
      this->hp = std::min(hp + hp_incr, hp_max);
   }
   
   void incrementMp(int mp_incr)
   {
      this->mp = std::min(mp + mp_incr, mp_max);
   }
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
 * Item actor 
 **/
struct Item
   :  public Actor
{
   InventoryItem::Type item_index = InventoryItem::Type::None;
   
   Item()
   {
      Actor    ::type   = Actor::Type::Item;
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


} /* namespace game */

#endif /* QUEST_GAME_ACTOR_HPP_INCLUDED */
