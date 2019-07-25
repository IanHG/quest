#include "Item.hpp"

#include "Actor.hpp"

namespace Game
{

std::unique_ptr<InventoryItemContainer> InventoryItemContainer::create()
{
   std::unique_ptr<InventoryItemContainer> iic = std::unique_ptr<InventoryItemContainer>{ new InventoryItemContainer };

   // Default none item
   iic->items[InventoryItem::None]          = InventoryItem{"None\0"};
   // Healing potion
   iic->items[InventoryItem::HealingPotion] = InventoryItem{"Healing potion\0", [](Character& character){
      character.incrementHp(10);
   }};
   // Mana potion
   iic->items[InventoryItem::MagicPotion]   = InventoryItem{"Magic potion\0", [](Character& character){
      character.incrementMp(10);
   }};
   // Key
   iic->items[InventoryItem::Key] = InventoryItem{"Key\0"};

   return iic;
}

std::unique_ptr<InventoryItemContainer> InventoryItemContainer::instance = InventoryItemContainer::create();

} /* namespace Game */
