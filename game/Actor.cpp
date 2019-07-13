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

void Item::interact(Actor& other)
{

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

} /* namespace game */
