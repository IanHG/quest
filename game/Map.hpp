#pragma once
#ifndef QUEST_GAME_MAP_HPP_INCLUDED
#define QUEST_GAME_MAP_HPP_INCLUDED

#include <functional>

#include "../graphics/Sprite.hpp"
#include "../graphics/Gui.hpp"
#include "../game/Actor.hpp"

namespace Game
{

/**
 *
 **/
struct Environment
{
   // Sprite
   Graphics::SpriteProxy sprite = Graphics::SpriteContainer::instance->getSprite(' ');
   
   // Environment characteristics
   bool passable = true;
   bool pushable = false;
   bool toxic    = false;
   
   // On move over
   using function_type    = std::function<void(Environment&, Actor&)>;
   function_type move_on  = [](Environment& env, Actor& actor) {  };
   function_type move_off = [](Environment& env, Actor& actor) {  };
   
   // Draw environment sprite
   void draw(WINDOW* win, int x, int y) const
   {
      sprite->draw(win, x, y);
   }

   void onMoveOn(Actor& actor)
   {
      this->move_on(*this, actor);
   }
   
   void onMoveOff(Actor& actor)
   {
      this->move_off(*this, actor);
   }

   // Create field
   static Environment create(char c)
   {
      switch(c)
      {
         case 'M':
         case 'W':
         case 'F':
         case '*':
         case '#':
            return Environment{Graphics::SpriteContainer::instance->getSprite(c), false};
         case ' ':
         case 'w':
         case 'm':
         case '0':
         case '1':
         default:
            return Environment{Graphics::SpriteContainer::instance->getSprite(c), true};
      }
   }
};

/**
 * Game map
 **/
struct Map
{
   using border_window_ptr = std::unique_ptr<Graphics::BorderWindow>;
   using EnvironmentArray  = std::unique_ptr<Environment[]>;
   using WindowIndex       = Graphics::Gui::WindowIndex;

   // Size of map
   int x_size = 0;
   int y_size = 0;
   
   // Off set when drawing
   int x_offset = 0;
   int y_offset = 0;
    
   // Map
   EnvironmentArray m_map          = EnvironmentArray{nullptr};
   WindowIndex      m_window_index = Graphics::Gui::WindowIndex{0};

   Map()  = default;
   ~Map() = default;
   
   // Move ctor and move assignment
   Map(Map&&)            = default;
   Map& operator=(Map&&) = default;
   
   // Check move
   bool isMoveOkay(const Actor& actor, int y, int x) const;

   void moveOn(Actor& actor)
   {
      m_map[actor.y + y_size * actor.x].onMoveOn(actor);
   }
   
   void moveOff(Actor& actor)
   {
      m_map[actor.y + y_size * actor.x].onMoveOff(actor);
   }

   // Draw map
   void draw() const;

   // Load map from file
   static Map load(const std::string& map_name); //, const std::string& map_path = );
};

} /* namespace Game */

#endif /* QUEST_GAME_MAP_HPP_INCLUDED */
