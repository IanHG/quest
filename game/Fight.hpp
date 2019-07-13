#pragma once
#ifndef QUEST_GAME_FIGHT_HPP_INCLUDED
#define QUEST_GAME_FIGHT_HPP_INCLUDED

#include <memory>

#include "../graphics/Gui.hpp"
#include "Actor.hpp"

namespace Game
{

struct Fight
{
   Graphics::Gui::WindowIndex window_index = Graphics::Gui::WindowIndex{-1};
   
   enum HitType { miss, graze, hit, crit };

   bool players_turn = true;
   bool player_won   = false;
   bool fight_ended  = false;
   bool status_end   = false;

   bool action       = false;
   bool enemy_action = false;
   std::string last_action       = std::string{""};
   std::string last_enemy_action = std::string{""};

   void start(Player& player, Character& other);

   void attack(Character& attacker, Character& defender);

   void setAttackAction(HitType hit_type, int damage, Character& attacker);

   void draw() const;
   
   void drawStatus() const;

   bool ended() const
   {
      return this->fight_ended;
   }
};

}

#endif /* QUEST_GAME_FIGHT_HPP_INCLUDED */
