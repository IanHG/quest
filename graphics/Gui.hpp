#pragma once
#ifndef QUEST_GRAPHICS_GUI_HPP_INCLUDED
#define QUEST_GRAPHICS_GUI_HPP_INCLUDED

#include "Window.hpp"

namespace Graphics
{

struct Gui
{
   using border_window_ptr = BorderWindow*;

   border_window_ptr main;
   border_window_ptr lore;
   border_window_ptr stats;
   border_window_ptr status;

   void initialize()
   {
      int height  = LINES;
	   int width   = COLS * 0.8;
      main = border_window_ptr{ new BorderWindow(nullptr, height, width, 0, 0) };

      int half_height  = LINES / 2 - 4;
      int status_width = COLS - width;
      lore   = border_window_ptr{ new BorderWindow(nullptr, half_height, status_width, width, 0) };
      stats  = border_window_ptr{ new BorderWindow(nullptr, half_height, status_width, width, half_height) };
      status = border_window_ptr{ new BorderWindow(nullptr, LINES - 2*half_height , status_width, width, 2*half_height) };
   }

   WINDOW* get_main()
   {
      return main->win;
   }

   void message(const std::string& msg)
   {
      wprintw(status->win, msg.c_str());
   }

   void finalize()
   {
      delete main;
      delete lore;
      delete stats;
      delete status;
   }

   void refresh()
   {
      main  ->refresh();
      status->refresh();
   }

   using gui_ptr_type = std::unique_ptr<Gui>;

   static void create()
   {
      instance = gui_ptr_type{ new Gui{} };
   }
   
   static gui_ptr_type instance;
};

inline Gui::instance = gui_ptr_type{nullptr};

} /* namespace Graphics */

#endif /* QUEST_GRAPHICS_GUI_HPP_INCLUDED */
