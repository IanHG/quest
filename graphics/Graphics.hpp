#pragma once
#ifndef QUEST_GRAPHICS_GRAPHICS_HPP_INCLUDED
#define QUEST_GRAPHICS_GRAPHICS_HPP_INCLUDED

#include <curses.h>

namespace Graphics
{

enum COLOR : int 
   {  BLACK
   ,  RED
   ,  GREEN
   ,  YELLOW
   ,  BLUE
   ,  MAGENTA
   ,  CYAN
   ,  WHITE
   ,  DARK_RED
   ,  DARKER_RED
   ,  LIGHT_RED
   ,  SIZE
   };

extern int color_pairs[COLOR::SIZE][COLOR::SIZE];

using CharType = chtype;

/**
 * Initialize graphics
 **/
void initialize();

/**
 * Finialize graphics
 **/
void finalize();

/**
 *
 **/
void drawCell(WINDOW* win, int x, int y, CharType symbol, int color_pair);

} /* namespace Graphics */

#endif /* QUEST_GRAPHICS_GRAPHICS_HPP_INCLUDED */
