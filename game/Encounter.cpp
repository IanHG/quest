#include "Encounter.hpp"

#include "../graphics/Gui.hpp"

#include "Actor.hpp"

namespace Game
{

/**
 * Start encounter
 **/
void Encounter::start(Player& player) 
{
}

/**
 * Start conversation encounter
 **/
void Conversation::start(Player& player)
{
   //ConversationPoint* conversation_point = this->conversation[0];

   while(true)
   {
      // Display conversation and choices
      //Graphics::Gui::displayConversation();
      
      // Take player choice

      // Update conversation tree
   }
}

} /* namespace Game */
