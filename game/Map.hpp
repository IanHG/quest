#pragma once
#ifndef QUEST_GAME_MAP_HPP_INCLUDED
#define QUEST_GAME_MAP_HPP_INCLUDED

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
   void draw(WINDOW* win, int x, int y)
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
      
   std::string                    m_map_name   = std::string{"binary.map"};
   std::unique_ptr<Environment[]> m_map        = field_ptr_type{nullptr};
   border_window_ptr              m_map_window = border_window_ptr{nullptr};

   Map()
   {
   }
   
   ~Map()
   {
   }

   bool is_move_okay(const Actor& actor, int y, int x)
   {
      if(actor.noclip)
      {
         return true;
      }

      if((y < 0) || (y >= y_size) || (x < 0) || (x >= x_size))
      {
         return false;
      }

      if(actor.flying)
      {
         return true;
      }

      if(!m_map[y + y_size * x].passable)
      {
         return false;
      }

      return true;
   }

   void draw()
   {
      for(int x = 0; x < x_size; ++x)
      {
         for(int y = 0; y < y_size; ++y)
         {
            m_map[y + y_size * x].draw(m_map_window->win, x, y);
         }
      }
   }

   void refresh()
   {
      m_map_window->refresh();
   }
   
   // Load map from file
   static Map load(const std::string& map_name); //, const std::string& map_path = );
};

} /* namespace Game */

#endif /* QUEST_GAME_MAP_HPP_INCLUDED */
