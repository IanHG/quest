#pragma once
#ifndef QUEST_GAME_MAP_HPP_INCLUDED
#define QUEST_GAME_MAP_HPP_INCLUDED

#include <functional>

#include "../graphics/Sprite.hpp"
#include "../graphics/Window.hpp"
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
   using function_type = std::function<void(Actor&)>;
   function_type move_over = [](Actor&){};
   
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
   using field_ptr_type    = std::unique_ptr<Environment[]>;

   // Size of map
   int x_size = 0;
   int y_size = 0;
   
   // Off set when drawing
   int x_offset = 0;
   int y_offset = 0;
    
   // Map
   std::unique_ptr<Environment[]> m_map        = field_ptr_type{nullptr};
   border_window_ptr              m_map_window = border_window_ptr{nullptr};

   Map()  = default;
   ~Map() = default;

   Map(Map&&)            = default;
   Map& operator=(Map&&) = default;
   
   // Check move
   bool isMoveOkay(const Actor& actor, int y, int x) const;

   // Draw map
   //void draw(WINDOW* win) const;
   void draw() const;

   // Refresh map framebuffer
   void refresh();
   
   // Load map from file
   static Map load(const std::string& map_name); //, const std::string& map_path = );
};

} /* namespace Game */

#endif /* QUEST_GAME_MAP_HPP_INCLUDED */
