#include "Fight.hpp"

#include "../graphics/Gui.hpp"
#include "../engine/Engine.hpp"
#include "../engine/Keyboard.hpp"

#include "Game.hpp"
#include "Actor.hpp"
#include "Roll.hpp"
#include "../event_handler.hpp"

namespace Game
{

/**
 *
 **/
void Fight::draw() const
{
   auto window = Graphics::Gui::getWindow(this->window_index);

   if(window)
   {
      auto win = window->getWindow();

      werase(win);
      wprintw(win, " \n");
      wprintw(win, " You are in a fight!!\n");
      wprintw(win, " \n");
      wprintw(win, " 1. Attack\n");
      wprintw(win, " 2. Retreat\n");
      wprintw(win, " \n");

      if(this->action)
      {
         wprintw(win, this->last_action.c_str());
      }

      if(this->enemy_action)
      {
         wprintw(win, " \n");
         wprintw(win, this->last_enemy_action.c_str());
         wprintw(win, " \n");
      }

      if(this->fight_ended)
      {
         if(this->player_won)
         {
            wprintw(win, " \n");
            wprintw(win, " You won the fight!\n");
            wprintw(win, " \n");
         }
         else
         {
            wprintw(win, " \n");
            wprintw(win, " You lost the fight! Game over!\n");
            wprintw(win, " \n");
         }
      }
   }
}

///**
// *
// **/
//void Fight::drawStatus() const
//{
//   auto window = Graphics::Gui::getWindow(this->window_index);
//
//   if(window)
//   {
//      auto win = window->getWindow();
//
//      werase(win);
//      if(this->player_won)
//      {
//         wprintw(win, " \n");
//         wprintw(win, " You won the fight!\n");
//         wprintw(win, " \n");
//      }
//      else
//      {
//         wprintw(win, " \n");
//         wprintw(win, " You lost the fight! Game over!\n");
//         wprintw(win, " \n");
//      }
//   }
//}

/**
 *
 **/
void Fight::setAttackAction(HitType hit_type, int damage, Character& attacker)
{
   Player* player = dynamic_cast<Player*>(&attacker);


   if(player)
   {
      switch(hit_type)
      {
         case HitType::miss:
            this->last_action = " You miss!?!";
            break;
         case HitType::graze:
            this->last_action = " You graze for " + std::to_string(damage) + " damage... ";
            break;
         case HitType::hit:
            this->last_action = " You attack for " + std::to_string(damage) + " damage. ";
            break;
         case HitType::crit:
            this->last_action = " CRITICAL HIT! You attack for " + std::to_string(damage) + " damage! ";
            break;
      }
      
      this->action = true;
   }
   else
   {
      switch(hit_type)
      {
         case HitType::miss:
            this->last_enemy_action = " Enemy misses!!!";
            break;
         case HitType::graze:
            this->last_enemy_action = " Enemy grazes you for " + std::to_string(damage) + " damage... ";
            break;
         case HitType::hit:
            this->last_enemy_action = " Enemy attacks you for " + std::to_string(damage) + " damage. ";
            break;
         case HitType::crit:
            this->last_enemy_action = " CRITICAL HIT! Enemy attacks you for " + std::to_string(damage) + " damage! ";
            break;
      }

      this->enemy_action = true;
   }

}

/**
 *
 **/
void Fight::attack(Character& attacker, Character& defender)
{
   int  hit_dice_roll = rollDie(100);
   int  difference    = hit_dice_roll + attacker.dexterity - defender.dexterity;

   HitType hit_type  = miss;
   if(difference < 30)
   {
      hit_type = HitType::miss;
   }
   else if(difference < 50)
   {
      hit_type = HitType::graze;
   }
   else if(difference < 80)
   {
      hit_type = HitType::hit;
   }
   else
   {
      hit_type = HitType::crit;
   }
   
   int damage_dice_roll = rollDie(6);
   int damage_base      = std::max(damage_dice_roll + attacker.strength - defender.strength, 0);
   int damage           = 0;

   switch(hit_type)
   {
      case HitType::miss:
         damage = 0;
         break;
      case HitType::graze:
         damage = damage_base * 0.5;
         break;
      case HitType::hit:
         damage = damage_base * 1;
         break;
      case HitType::crit:
         damage = damage_base * 2;
         break;
   }

   defender.hp = defender.hp - damage;

   this->setAttackAction(hit_type, damage, attacker);
}

/**
 *
 **/
void Fight::start(Player& player, Character& other)
{
   // Create window to display fight
   auto main_window   = Graphics::Gui::getWindow(Graphics::Gui::Window::MAIN);
   this->window_index = Graphics::Gui::createWindow
      (  Graphics::Gui::Window::MAIN
      ,  main_window->xMax()
      ,  main_window->yMax() / 2
      ,  0
      ,  0
      );

   // Create event listener for encounter
   MultiEventRegisterer<event_handler<char, void(const char&)> > mer;
   
   mer.addEvent('1', [this, &player, &other](const char&){
      if(this->players_turn)
      {
         this->attack(player, other);
         this->players_turn = false;
      }
   });
   
   mer.addEvent('2', [this, &player, &other](const char&){
      this->fight_ended = true;
   });

   mer.registerEvents(Engine::Keyboard::instance());

   // Start game loop on dialogue
   Engine::gameLoop([this, &player, &other](){
      this->draw();
      player.drawStats();

      if(!this->players_turn)
      {
         this->attack(other, player);
         this->players_turn = true;
      }

      if(player.hp <= 0)
      {
         this->fight_ended = true;
      }
      if(other.hp <= 0)
      {
         this->player_won  = true;
         this->fight_ended = true;
      }

      return !(this->ended());
   });
   
   // De register events
   mer.deregisterEvents(Engine::Keyboard::instance());

   // Output status of fight
   MultiEventRegisterer<event_handler<char, void(const char&)> > fight_status_mer;

   fight_status_mer.addEvent('1', [this, &player, &other](const char&){
      this->status_end = true;
   });
   fight_status_mer.registerEvents(Engine::Keyboard::instance());

   Engine::gameLoop([this](){
      this->draw();

      return !(this->status_end);
   });
   
   fight_status_mer.deregisterEvents(Engine::Keyboard::instance());

   if(this->player_won)
   {
      player.giveXp(1);

      instance->removeActor(other.index);
   }
   else
   {
      instance->gameOver();
   }

   // Destroy window
   Graphics::Gui::destroyWindow(this->window_index);
}

} /* namespace Game */
