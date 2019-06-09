#pragma once 
#ifndef QUEST_SPRITE_HPP_INCLUDED
#define QUEST_SPRITE_HPP_INCLUDED

#include <curses.h>
#include <cassert>

#define GRASS    ' '
#define EMPTY    ' '
#define WATER	  '~'
#define MOUNTAIN '^'
#define PLAYER	  '@'
#define TREE     '*'
#define GRAIL    'G'
#define ACTOR    'A'
#define ORC      'O'
#define TORCH     '*'

#define BINARY_ZERO ' '
#define BINARY_ONE  ' '

#define DEFAULT_COLOR 1
#define PLAYER_COLOR  2
#define GRAIL_COLOR   3
#define ENEMY_COLOR   4
#define WATER_COLOR   5
#define SMALL_WATER_COLOR   6
#define TREE_COLOR    7
#define BINARY_ZERO_COLOR 8
#define BINARY_ONE_COLOR  9
#define TORCH_COLOR  10

/**
 *
 **/
inline void initialize_colors()
{
}

/**
 *
 **/
struct sprite_type
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
struct sprite_proxy
{
   const sprite_type* m_sprite;

   sprite_proxy(const sprite_type* const sprite)
      :  m_sprite(sprite)
   {
   }

   sprite_proxy(sprite_proxy&& other)
      :  m_sprite(other.m_sprite)
   {
   }

   sprite_proxy& operator=(sprite_proxy&& other)
   {
      m_sprite = other.m_sprite;
      return *this;
   }

   const sprite_type* const operator->() const
   {
      return m_sprite;
   }
};

/**
 *
 **/
struct sprite_container
{
   using container_type = std::vector<sprite_type>;
   using code_type      = std::vector<char>;

   container_type m_container;
   code_type      m_code;

   sprite_container()
   {
      // Initialize ncurses colours
      initialize_colors();
      
      // Empty sprite
      m_code      .emplace_back(' ');
      m_container .emplace_back(sprite_type{EMPTY, DEFAULT_COLOR});
      // Player sprite
      m_code      .emplace_back('P');
      m_container .emplace_back(sprite_type{PLAYER, PLAYER_COLOR});
      // Grail
      m_code      .emplace_back('G');
      m_container .emplace_back(sprite_type{GRAIL, GRAIL_COLOR});
      // Mountain
      m_code      .emplace_back('M');
      m_container .emplace_back(sprite_type{MOUNTAIN, ENEMY_COLOR});
      // Water
      m_code      .emplace_back('W');
      m_container .emplace_back(sprite_type{WATER, WATER_COLOR});
      // Water
      m_code      .emplace_back('w');
      m_container .emplace_back(sprite_type{WATER, SMALL_WATER_COLOR});
      // Tree/forest
      m_code      .emplace_back('F');
      m_container .emplace_back(sprite_type{TREE, TREE_COLOR});
      // Tree/forest
      m_code      .emplace_back('*');
      m_container .emplace_back(sprite_type{TORCH, TORCH_COLOR});

      // 0
      m_code      .emplace_back('0');
      m_container .emplace_back(sprite_type{BINARY_ZERO, BINARY_ONE_COLOR});
      // 1
      m_code      .emplace_back('1');
      m_container .emplace_back(sprite_type{BINARY_ONE, BINARY_ZERO_COLOR});

      // Orc
      m_code      .emplace_back('O');
      m_container .emplace_back(sprite_type{ORC, ENEMY_COLOR});
   }

   sprite_proxy get_sprite(char c) const
   {
      for(std::size_t i = 0; i < m_code.size(); ++i)
      {
         if(m_code[i] == c)
         {
            return sprite_proxy{ &(m_container[i]) };
         }
      }
      return sprite_proxy{ &(m_container[0]) };
   }

   static void create()
   {
      instance = std::unique_ptr<sprite_container>{ new sprite_container };
   }

   static std::unique_ptr<sprite_container> instance;
};

inline std::unique_ptr<sprite_container> sprite_container::instance = std::unique_ptr<sprite_container>{nullptr};

#endif /* QUEST_SPRITE_HPP_INCLUDED */
