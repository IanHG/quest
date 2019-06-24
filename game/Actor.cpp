#include "Actor.hpp"

#include <iostream>

#include "Game.hpp"

namespace Game
{

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
      performMove(*this, x_new, y_new);
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

} /* namespace game */
