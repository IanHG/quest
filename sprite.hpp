#pragma once 
#ifndef QUEST_SPRITE_HPP_INCLUDED
#define QUEST_SPRITE_HPP_INCLUDED

#include <curses.h>

#define GRASS    ' '
#define EMPTY    '.'
#define WATER	  '~'
#define MOUNTAIN '^'
#define PLAYER	  '@'
#define GRAIL    'G'
#define ACTOR    'A'
#define ORC      'O'

#define DEFAULT_COLOR 1
#define PLAYER_COLOR  2
#define GRAIL_COLOR   3
#define ENEMY_COLOR   4
#define WATER_COLOR   5

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
   const sprite_type* const m_sprite;

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
      return sprite_proxy{ nullptr };
   }

   static void create()
   {
      instance = std::unique_ptr<sprite_container>{ new sprite_container };
   }

   static std::unique_ptr<sprite_container> instance;
};

inline std::unique_ptr<sprite_container> sprite_container::instance = std::unique_ptr<sprite_container>{nullptr};

#endif /* QUEST_SPRITE_HPP_INCLUDED */