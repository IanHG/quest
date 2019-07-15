#pragma once 
#ifndef QUEST_GRAPHICS_SPRITE_HPP_INCLUDED
#define QUEST_GRAPHICS_SPRITE_HPP_INCLUDED

#include "TypeDefs.hpp"
#include "Graphics.hpp"

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
   enum : int
   {  empty  = 0
   ,  player
   ,  size
   };

   char  symbol = EMPTY;
   int   color  = DEFAULT_COLOR;
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
      m_container .emplace_back(Sprite{EMPTY, color_pairs[COLOR::BLACK][COLOR::BLACK]});
      // Player sprite
      m_code      .emplace_back('P');
      m_container .emplace_back(Sprite{PLAYER, color_pairs[COLOR_CYAN][COLOR_BLACK]});
      // Grail
      m_code      .emplace_back('G');
      m_container .emplace_back(Sprite{GRAIL, color_pairs[COLOR::YELLOW][COLOR::BLACK]});
      // Mountain
      m_code      .emplace_back('M');
      m_container .emplace_back(Sprite{MOUNTAIN, color_pairs[COLOR::WHITE][COLOR::RED]});
      // Water
      m_code      .emplace_back('W');
      m_container .emplace_back(Sprite{WATER, color_pairs[COLOR::CYAN][COLOR::BLUE]});
      // Water
      m_code      .emplace_back('w');
      m_container .emplace_back(Sprite{WATER, color_pairs[COLOR::BLUE][COLOR::BLACK]});
      // Tree/forest
      m_code      .emplace_back('F');
      m_container .emplace_back(Sprite{TREE, color_pairs[COLOR::RED][COLOR::GREEN]});
      // Tree/forest
      m_code      .emplace_back('*');
      m_container .emplace_back(Sprite{TORCH, color_pairs[COLOR::YELLOW][COLOR::BLACK]});
      // Tree/forest
      m_code      .emplace_back('#');
      m_container .emplace_back(Sprite{WALL, color_pairs[COLOR::WHITE][COLOR::RED]});
      // Tree/forest
      m_code      .emplace_back('P');
      m_container .emplace_back(Sprite{WALL, color_pairs[COLOR::WHITE][COLOR::BLUE]});
      // Tree/forest
      m_code      .emplace_back('D');
      m_container .emplace_back(Sprite{WALL, color_pairs[COLOR::GREEN][COLOR::BLACK]});

      // 0
      m_code      .emplace_back('0');
      m_container .emplace_back(Sprite{BINARY_ZERO, color_pairs[COLOR::BLACK][COLOR::DARKER_RED]});
      // 1
      m_code      .emplace_back('1');
      m_container .emplace_back(Sprite{BINARY_ONE, color_pairs[COLOR::BLACK][COLOR::BLACK]});

      // Orc
      m_code      .emplace_back('O');
      m_container .emplace_back(Sprite{ORC, color_pairs[COLOR::RED][COLOR::BLACK]});
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

inline SpriteProxy getSprite(char c)
{
   return SpriteContainer::instance->getSprite(c);
}

} /* namespace Graphics */

#endif /* QUEST_GRAPHICS_SPRITE_HPP_INCLUDED */
