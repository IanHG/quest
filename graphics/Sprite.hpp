#pragma once 
#ifndef QUEST_GRAPHICS_SPRITE_HPP_INCLUDED
#define QUEST_GRAPHICS_SPRITE_HPP_INCLUDED


#include "TypeDefs.hpp"
#include <cassert>
#include <vector>
#include <memory>

namespace Graphics
{

/**
 *
 **/
struct Sprite
{
   char  symbol = EMPTY;
   int   color  = DEFAULT_COLOR;

   void draw(WINDOW* win, int x, int y) const
   {
      wattron (win, COLOR_PAIR(color));
	   mvwaddch(win, y, x, this->symbol);
      wattroff(win, COLOR_PAIR(color));
   }
};

/**
 *
 **/
struct SpriteProxy
{
   const Sprite* m_sprite;

   SpriteProxy(const Sprite* const sprite)
      :  m_sprite(sprite)
   {
   }

   SpriteProxy(SpriteProxy&& other)
      :  m_sprite(other.m_sprite)
   {
   }

   SpriteProxy& operator=(SpriteProxy&& other)
   {
      m_sprite = other.m_sprite;
      return *this;
   }

   const Sprite* const operator->() const
   {
      return m_sprite;
   }
};

/**
 *
 **/
struct SpriteContainer
{
   using container_type = std::vector<Sprite>;
   using code_type      = std::vector<char>;

   container_type m_container;
   code_type      m_code;
   
   /**
    * Constructor
    **/
   SpriteContainer()
   {
      // Empty sprite
      m_code      .emplace_back(' ');
      m_container .emplace_back(Sprite{EMPTY, DEFAULT_COLOR});
      // Player sprite
      m_code      .emplace_back('P');
      m_container .emplace_back(Sprite{PLAYER, PLAYER_COLOR});
      // Grail
      m_code      .emplace_back('G');
      m_container .emplace_back(Sprite{GRAIL, GRAIL_COLOR});
      // Mountain
      m_code      .emplace_back('M');
      m_container .emplace_back(Sprite{MOUNTAIN, ENEMY_COLOR});
      // Water
      m_code      .emplace_back('W');
      m_container .emplace_back(Sprite{WATER, WATER_COLOR});
      // Water
      m_code      .emplace_back('w');
      m_container .emplace_back(Sprite{WATER, SMALL_WATER_COLOR});
      // Tree/forest
      m_code      .emplace_back('F');
      m_container .emplace_back(Sprite{TREE, TREE_COLOR});
      // Tree/forest
      m_code      .emplace_back('*');
      m_container .emplace_back(Sprite{TORCH, TORCH_COLOR});

      // 0
      m_code      .emplace_back('0');
      m_container .emplace_back(Sprite{BINARY_ZERO, BINARY_ONE_COLOR});
      // 1
      m_code      .emplace_back('1');
      m_container .emplace_back(Sprite{BINARY_ONE, BINARY_ZERO_COLOR});

      // Orc
      m_code      .emplace_back('O');
      m_container .emplace_back(Sprite{ORC, ENEMY_COLOR});
   }

   /**
    * Get sprite 
    **/
   SpriteProxy getSprite(char c) const
   {
      for(std::size_t i = 0; i < m_code.size(); ++i)
      {
         if(m_code[i] == c)
         {
            return SpriteProxy{ &(m_container[i]) };
         }
      }
      return SpriteProxy{ &(m_container[0]) };
   }
   
   /**
    * Initialize/create sprite container
    **/
   static void create()
   {
      instance = std::unique_ptr<SpriteContainer>{ new SpriteContainer };
   }
   
   // Pointer to sprite container instance
   static std::unique_ptr<SpriteContainer> instance;
};

inline std::unique_ptr<SpriteContainer> SpriteContainer::instance = std::unique_ptr<SpriteContainer>{nullptr};

} /* namespace Graphics */

#endif /* QUEST_GRAPHICS_SPRITE_HPP_INCLUDED */
