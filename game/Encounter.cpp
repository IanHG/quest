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

using Engine::ichtype;

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
   MultiEventRegisterer<event_handler<ichtype, void()> > mer;
   
   for(int i = 1; i < 10; ++i)
   {
      mer.addEvent(ichtype(i + 48), [&dialog, i](){
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

   player.giveXp(1);
   
   // De register events
   mer.deregisterEvents(Engine::Keyboard::instance());
}

} /* namespace Game */
