#include "Actor.hpp"

namespace Game
{

void Npc::interact(Actor& other)
{
   if(other.type == Actor::player)
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
   this->interacting = false;
}

} /* namespace game */
