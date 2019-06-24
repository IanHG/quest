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
   using function_type    = std::function<void(Actor&)>;
   function_type move_on  = [](Actor&){};
   function_type move_off = [](Actor&){};
   
   // Draw environment sprite
   void draw(WINDOW* win, int x, int y) const
   {
      sprite->draw(win, x, y);
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

   // Size of map
   int x_size = 0;
   int y_size = 0;
   
   // Off set when drawing
   int x_offset = 0;
   int y_offset = 0;
    
   // Map
   EnvironmentArray m_map          = EnvironmentArray{nullptr};
   //border_window_ptr m_map_window = border_window_ptr{nullptr};
   Graphics::Gui::WindowIndex m_window_index = Graphics::Gui::WindowIndex{0};

   Map()  = default;
   ~Map() = default;
   
   // Move ctor and move assignment
   Map(Map&&)            = default;
   Map& operator=(Map&&) = default;
   
   // Check move
   bool isMoveOkay(const Actor& actor, int y, int x) const;

   // Draw map
   //void draw(WINDOW* win) const;
   void draw() const;

   // Load map from file
   static Map load(const std::string& map_name); //, const std::string& map_path = );
};

} /* namespace Game */

#endif /* QUEST_GAME_MAP_HPP_INCLUDED */
