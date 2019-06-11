#pragma once
#ifndef QUEST_GRAPHICS_WINDOW_HPP_INCLUDED
#define QUEST_GRAPHICS_WINDOW_HPP_INCLUDED

#include "TypeDefs.hpp"

namespace Graphics
{

struct BorderWindow
{
   WINDOW* win_border = nullptr;
   WINDOW* win        = nullptr;
   int xmax           = 0;
   int ymax           = 0;

   BorderWindow(WINDOW* parent_win, int height, int width, int  startx, int starty)
   {
      // create border window
      if(parent_win)
      {
         win_border = derwin(parent_win, height, width, starty, startx);
      }
      else
      {
         win_border = create_newwin(height, width, starty, startx);
      }
      box(win_border, 0, 0);
      wrefresh(win_border);

      // create actual window
      win        = derwin(win_border, height - 2, width - 2, 1, 1);
      wrefresh(win);

      // get max x and y
      getmaxyx(win, ymax, xmax);
   }

   ~BorderWindow()
   {
      destroy_win(win);

	   wborder(win_border, ' ', ' ', ' ',' ',' ',' ',' ',' ');
      destroy_win(win_border);
   }

   void refresh()
   {
      wrefresh(win_border);
      wrefresh(win);
   }
};

} /* namespace Graphics */

#endif /* QUEST_GRAPHICS_WINDOW_HPP_INCLUDED */
