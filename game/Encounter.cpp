#include "Encounter.hpp"

#include "../graphics/Gui.hpp"
#include "../engine/Engine.hpp"
#include "../engine/Keyboard.hpp"

#include "Actor.hpp"
//#include "Conversation.hpp"
//
#include "../event_handler.hpp"

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
   MultiEventRegisterer<event_handler<char, void(const char&)> > mer;
   //Conversation         conversation Conversation::load(this->tag);

   //mer.addEvent('1', [&conversation](const char&){
   bool end = true;
   mer.addEvent('1', [&end](const char&){
      //conversation.option(1);
      end = false;
   });

   mer.registerEvents(Engine::Keyboard::instance());
   
   //Engine::gameLoop([&conversation](){
   Engine::gameLoop([&end](){
      //conversation.update();
      Graphics::Gui::instance->message("MOFO!\n");

      return end;
   });

   mer.deregisterEvents(Engine::Keyboard::instance());
}

} /* namespace Game */
