#pragma once
#ifndef QUEST_GAME_ITEM_HPP_INCLUDED
#define QUEST_GAME_ITEM_HPP_INCLUDED

#include <functional>
#include <memory>

//#include "Actor.hpp"

namespace Game
{

struct Character;

struct InventoryItem
{
   enum Type : int 
   {  None
   ,  HealingPotion
   ,  MagicPotion
   ,  Sword
   ,  Size
   };

   char name[56];
   std::function<void(Character&)> on_usage;
};

struct InventoryItemContainer
{
   InventoryItem items[InventoryItem::Type::Size];

   static std::unique_ptr<InventoryItemContainer> create();

   static std::unique_ptr<InventoryItemContainer> instance;

   static const InventoryItem& getItem(InventoryItem::Type type)
   {
      return instance->items[type];
   }
};

} /* namespace Game */

#endif /* QUEST_GAME_ITEM_HPP_INCLUDED */
