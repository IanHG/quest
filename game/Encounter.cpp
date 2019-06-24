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
   // Load dialog
   Dialog dialog = Dialog::load(std::string("default"));

   // Create event listener for encounter
   MultiEventRegisterer<event_handler<char, void(const char&)> > mer;
   
   for(int i = 1; i < 10; ++i)
   {
      mer.addEvent(char(i + 48), [&dialog, i](const char&){
         dialog.option(i);
      });
   }

   mer.registerEvents(Engine::Keyboard::instance());
   
   // Start game loop on dialogue
   Engine::gameLoop([&dialog](){
      //dialog.draw(Gui::instance->main);
      dialog.draw();

      return dialog.ended();
   });
   
   // De register events
   mer.deregisterEvents(Engine::Keyboard::instance());
}

} /* namespace Game */
