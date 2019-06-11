#pragma once
#ifndef QUEST_GAME_CHEATS_HPP_INCLUDED
#define QUEST_GAME_CHEATS_HPP_INCLUDED

#include "../engine/Keyboard.hpp"

namespace Game
{

struct Cheats
{
   std::vector<std::unique_ptr<Engine::KeyboardCombo> > combos;

   Cheats()
   {
   }

   void addKeyboardCombo(std::vector<char> combo, std::function<void()> function)
   {
      auto& kb = Engine::Keyboard::instance();
      combos.emplace_back(std::unique_ptr<Engine::KeyboardCombo>{ new Engine::KeyboardCombo{combo, function, kb} });
   }

   void enable()
   {
      for(auto& kc : combos)
      {
         kc->enable();
      }
   }

   void disable()
   {
      for(auto& kc : combos)
      {
         kc->disable();
      }
   }
};

} /* namespace Game */

#endif /* QUEST_GAME_CHEATS_HPP_INCLUDED */
