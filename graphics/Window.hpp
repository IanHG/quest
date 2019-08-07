#pragma once
#ifndef QUEST_GRAPHICS_WINDOW_HPP_INCLUDED
#define QUEST_GRAPHICS_WINDOW_HPP_INCLUDED

#include "TypeDefs.hpp"

#include <memory>

namespace Graphics
{

/**
 *
 **/
inline WINDOW* create_newwin(int height, int width, int starty, int startx)
{	
   WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	//wrefresh(local_win);		/* Show that box 		*/
   wnoutrefresh(local_win);

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
	//wrefresh(local_win);
   wnoutrefresh(local_win);
      
	delwin(local_win);
}

/**
 *
 **/
struct IWindow
{
   using SmartPtr = std::unique_ptr<IWindow>;
   using Ptr      = IWindow*;
   
   virtual ~IWindow()
   {
   }

   virtual WINDOW* getWindow() const
   {
      return nullptr;
   }
   
   virtual int xMax() const
   {
      return 0;
   }
   
   virtual int yMax() const
   {
      return 0;
   }

   virtual void draw()
   {
   }

   virtual void refresh()
   {
   }
};

/**
 *
 **/
struct Window
   :  public IWindow
{
   WINDOW* win = nullptr;
   int xmax    = 0;
   int ymax    = 0;

   Window(WINDOW* parent_win, int height, int width, int  startx, int starty)
   {
      // 
      if(parent_win)
      {
         win = derwin(parent_win, height, width, starty, startx);
      }
      else
      {
         win = create_newwin(height, width, starty, startx);
      }

      // get max x and y
      getmaxyx(win, ymax, xmax);
   }

   ~Window()
   {
      // Destroy window
      destroy_win(win);
   }

   WINDOW* getWindow() const
   {
      return win;
   }

   int xMax() const
   {
      return xmax;
   }

   int yMax() const
   {
      return ymax;
   }

   void draw()
   {
   }

   void refresh()
   {
      //wrefresh(win);
      wnoutrefresh(win);
   }
};

/**
 *
 **/
struct BorderWindow
   :  public IWindow
{
   WINDOW* win_border = nullptr;
   WINDOW* win        = nullptr;
   int xmax           = 0;
   int ymax           = 0;
   
   //! Constructor
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

      // create actual window
      win        = derwin(win_border, height - 2, width - 2, 1, 1);

      // get max x and y
      getmaxyx(win, ymax, xmax);
   }
   
   //! Destructor
   ~BorderWindow()
   {
      // Destroy window
      wclear(win);
      destroy_win(win);
      
      // Detroy border
	   wborder(win_border, ' ', ' ', ' ',' ',' ',' ',' ',' ');
      destroy_win(win_border);
   }
   
   //! Get the window
   WINDOW* getWindow() const
   {
      return win;
   }
   
   //! Get xmax
   int xMax() const
   {
      return xmax;
   }

   //! Get ymax
   int yMax() const
   {
      return ymax;
   }

   //!
   void draw()
   {
      box(win_border, 0, 0);
   }
   
   //! Refresh window
   void refresh()
   {
      wnoutrefresh(win_border);
      wnoutrefresh(win);
      //wrefresh(win_border);
      //wrefresh(win);
   }
};

} /* namespace Graphics */

#endif /* QUEST_GRAPHICS_WINDOW_HPP_INCLUDED */
