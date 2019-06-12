#include "Encounter.hpp"

#include "../graphics/Gui.hpp"
#include "../engine/Engine.hpp"
#include "../engine/Keyboard.hpp"

#include "Actor.hpp"
#include "Dialog.hpp"
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
   Dialog dialog = Dialog::load(std::string("default"));

   //mer.addEvent('1', [&conversation](const char&){
   mer.addEvent('1', [&dialog](const char&){
      dialog.option(1);
   });

   mer.registerEvents(Engine::Keyboard::instance());
   
   //Engine::gameLoop([&conversation](){
   Engine::gameLoop([&dialog](){
      dialog.draw();

      return dialog.ended();
   });

   mer.deregisterEvents(Engine::Keyboard::instance());
}

} /* namespace Game */
