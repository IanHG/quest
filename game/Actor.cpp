#include "Actor.hpp"

#include <iostream>

#include "Game.hpp"
#include "Map.hpp"
#include "Fight.hpp"

namespace Game
{

void Actor::draw(Map& map) const
{
   map.drawActor(*this);
}

void Pushable::interact(Actor& other)
{
   int x_new = this->x > other.x ? this->x + 1 
             : this->x < other.x ? this->x - 1
             : this->x;
   int y_new = this->y > other.y ? this->y + 1 
             : this->y < other.y ? this->y - 1
             : this->y;

   if(checkMove(*this, x_new, y_new))
   {
      int x_old = this->x;
      int y_old = this->y;

      performMove(*this, x_new, y_new);
      performMove(other, x_old, y_old);
   }
}

void Npc::interact(Actor& other)
{
   if(other.type == Actor::Type::Player)
   {
      Player* player = dynamic_cast<Player*>(&other);
      player->interacting = true;
      if(hostile)
      {
         // Attack
         Fight fight;
         fight.start(*player, *this);
      }
      else
      {
         // Conversation
         this->encounter.start(*player);
      }
      player->interacting = false;
   }
}


void Player::interact(Actor& other)
{
   this->interacting = true;
   Npc* npc = dynamic_cast<Npc*>(&other);
   if(npc)
   {
      if(npc->hostile)
      {
         // Attack
         Fight fight;
         fight.start(*this, *npc);
      }
      else
      {
         // Conversation
         npc->encounter.start(*this);
      }
   }
   else
   {
      other.interact(*this);
   }
   
   this->interacting = false;
}

void Player::drawStats() const
{
   auto stats_window = Graphics::Gui::getWindow(Graphics::Gui::Window::STATS);
   
   if(stats_window)
   {
      WINDOW* win = stats_window->getWindow();
      werase(win);
      wprintw(win, "  \n");
      wprintw(win, "  Strength    %i\n", this->strength);
      wprintw(win, "  Dexterity   %i\n", this->dexterity);
      wprintw(win, "  Wisdom      %i\n", this->intelligence);
      wprintw(win, "  \n");
      wprintw(win, "  HP          %i/%i\n", this->hp, this->hp_max);
      wprintw(win, "  MP          %i/%i\n", this->mp, this->mp_max);
      wprintw(win, "  \n");
      wprintw(win, "  Level       %i\n", this->level);
      wprintw(win, "  XP          %i/%i\n", this->xp, this->xp_for_next_level);
      wprintw(win, "  \n");
      for(int i = 0; i < int(this->inventory.size()); ++i)
      {
         if(inventory[i].item_count > 0)
         {
            auto& item = InventoryItemContainer::getItem(this->inventory[i].item_index);
            wprintw(win, " %i %s\n", inventory[i].item_count, item.name);
         }
      }
   }
}

void Player::giveXp(int xp)
{
   // Increment xp
   this->xp += xp;
}

void Player::checkLevelUp()
{
   // Check for level up
   if(this->xp >= this->xp_for_next_level)
   {
      // Perform level up
      this->strength += 1;

      // Increment xp for next level
      ++this->next_level;
      this->xp_for_next_level += this->next_level;
   }
}

void Player::checkStatus()
{
   if(this->hp <= 0)
   {
   }
}

void Player::update()
{
   this->checkStatus ();
   this->checkLevelUp();
}

void Item::interact(Actor& other)
{
   int pick_up = -1;
   
   // Create window to display pick up interaction
   auto main_window   = Graphics::Gui::getWindow(Graphics::Gui::Window::MAIN);
   auto window_index = Graphics::Gui::createWindow
      (  Graphics::Gui::Window::MAIN
      ,  main_window->xMax()
      ,  main_window->yMax() / 2
      ,  0
      ,  0
      );
   auto window = Graphics::Gui::getWindow(window_index);

   // Create event listener for encounter
   MultiEventRegisterer<event_handler<char, void(const char&)> > mer;
   
   mer.addEvent('1', [&pick_up](const char&){
      pick_up = 1;
   });
   
   mer.addEvent('2', [&pick_up](const char&){
      pick_up = 2;
   });

   mer.registerEvents(Engine::Keyboard::instance());

   Engine::gameLoop([&pick_up, &window, this](){
      if(window)
      {
         auto win = window->getWindow();

         werase(win);
         wprintw(win, " \n");
         for(int i = 0; i < this->n_items_max; ++i)
         {
            if(this->item_indices[i] != InventoryItem::Type::None)
            {
               auto& item  = InventoryItemContainer::getItem(this->item_indices[i]);
               wprintw(win, " You found a '%s'.", item.name);
            }
         }
         wprintw(win, " \n");
         wprintw(win, " 1. Pick up\n");
         wprintw(win, " 2. Leave it\n");
         wprintw(win, " \n");
      }

      return (pick_up == -1);
   });
   
   mer.deregisterEvents(Engine::Keyboard::instance());
   
   if(pick_up == 1)
   {
      Character* other_character = dynamic_cast<Character*>(&other);
      if(other_character)
      {
         for(int i = 0; i < this->n_items_max; ++i)
         {

            if(this->item_indices[i] != InventoryItem::Type::None)
            {
               other_character->pickUpItem(this->item_indices[i], 1);
            }
         }
         instance->removeActor(this->index);
      }
   }
   
   // Destroy window
   Graphics::Gui::destroyWindow(window_index);
}

Actor::SmartPtr Actor::createActor(const std::string& type, int x, int y)
{
   Actor::SmartPtr actor = Actor::SmartPtr{nullptr};

   if(type == "orc")
   {
      actor = Actor::create(Actor::Type::Npc, x, y);
      actor->sprite = Graphics::getSprite(Graphics::Sprite::Orc);

      Npc* npc = dynamic_cast<Npc*>(actor.get());
      if(npc)
      {
         npc->hostile = true;
      }
   }
   else if(type == "rock")
   {
      actor = Actor::create(Actor::Type::Pushable, x, y);
      actor->sprite = Graphics::getSprite(Graphics::Sprite::Rock);
   }
   else if(type == "healing_potion")
   {
      actor = Actor::create(Actor::Type::Item, x, y);
      actor->sprite = Graphics::getSprite(Graphics::Sprite::Chest);

      Item* item = dynamic_cast<Item*>(actor.get());
      item->item_indices[0] = InventoryItem::Type::HealingPotion;
   }
   
   return actor;
}

} /* namespace game */
