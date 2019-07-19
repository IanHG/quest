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

   static Type stringToType(const std::string& type_str)
   {
      if(type_str == "HealingPotion")
      {
         return Type::HealingPotion;
      }
      else if(type_str == "MagicPotion")
      {
         return Type::MagicPotion;
      }
      else if(type_str == "Sword")
      {
         return Type::Sword;
      }
      
      return Type::None;
   }

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
