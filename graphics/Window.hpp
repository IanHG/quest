#pragma once
#ifndef QUEST_GRAPHICS_WINDOW_HPP_INCLUDED
#define QUEST_GRAPHICS_WINDOW_HPP_INCLUDED

#include "TypeDefs.hpp"

namespace Graphics
{

/**
 *
 **/
inline WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

/**
 *
 **/
inline void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}

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
