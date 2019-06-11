#include "Actor.hpp"

namespace Game
{

void Npc::interact(Actor& other)
{
   if(other.type == Actor::player)
   {
      Player* player = dynamic_cast<Player*>(&other);
      if(hostile)
      {
         // Attack
      }
      else
      {
         // Conversation
         this->encounter.start(*player);
      }
   }
}


void Player::interact(Actor& other)
{
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
}

} /* namespace game */
