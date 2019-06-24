#pragma once
#ifndef QUEST_GRAPHICS_GUI_HPP_INCLUDED
#define QUEST_GRAPHICS_GUI_HPP_INCLUDED

#include <vector>
#include <cassert>
#include <iostream>

#include "Window.hpp"

namespace Graphics
{

struct Gui
{
   using BorderWindowPtr = BorderWindow*;
   using IWindowVec      = std::vector<IWindow::SmartPtr>;

   using WindowIndex     = int;
   enum Window : WindowIndex { ERROR = -2, NONE = -1, MAIN = 0, LORE, STATS, STATUS };

   IWindowVec windows;
   
   //! Initialize Gui
   void initialize()
   {
      // We limit ourselves to 10 windows
      windows.resize(10);
      
      // Create main window
      int height  = LINES;
	   int width   = COLS * 0.8;
      windows[Window::MAIN] = IWindow::SmartPtr{ new BorderWindow(nullptr, height, width, 0, 0) };
      
      // Create rest of windows
      int half_height  = LINES / 2 - 4;
      int status_width = COLS - width;
      windows[Window::LORE]   = IWindow::SmartPtr{ new BorderWindow(nullptr, half_height, status_width, width, 0) };
      windows[Window::STATS]  = IWindow::SmartPtr{ new BorderWindow(nullptr, half_height, status_width, width, half_height) };
      windows[Window::STATUS] = IWindow::SmartPtr{ new BorderWindow(nullptr, LINES - 2*half_height , status_width, width, 2*half_height) };
   }

   //! Get window by index
   static IWindow::Ptr getWindow(WindowIndex index)
   {
      if(instance->windows[index])
      {
         return instance->windows[index].get();
      }
      else
      {
         return nullptr;
      }
   }


   //! Create window
   static WindowIndex createWindow(WindowIndex parent, int x_size, int y_size, int x_offset, int y_offset)
   {
      if(parent >= 0)
      {
         for(decltype(instance->windows.size()) i = 0; i < instance->windows.size(); ++i)
         {
            if(!instance->windows[i])
            {
               instance->windows[i] = IWindow::SmartPtr
                  {  new BorderWindow
                     (  instance->windows[parent]->getWindow()
                     ,  y_size
                     ,  x_size
                     ,  x_offset
                     ,  y_offset
                     )
                  };
               return i;
            }
         }
      }
      else
      {
         // not implemented yet
         assert(false);
      }

      return Window::ERROR;
   }

   static void destroyWindow(WindowIndex window)
   {
      if((window >= 0) && (window < int(instance->windows.size())))
      {
         instance->windows[window] = IWindow::SmartPtr{ nullptr };
      }
   }

   void message(const std::string& msg)
   {
      wprintw(windows[Window::STATUS]->getWindow(), msg.c_str());
   }

   void finalize()
   {
   }

   //!
   void draw()
   {
      for(auto& win : windows)
      {
         if(win)
         {
            win->draw();
         }
      }
   }

   //! Refresh gui
   void refresh()
   {
      for(auto& win : windows)
      {
         if(win)
         {
            win->refresh();
         }
      }
      //main  ->refresh();
      //lore  ->refresh();
      //stats ->refresh();
      //status->refresh();
   }

   /**
    * Gui instance
    **/
   using gui_ptr_type = std::unique_ptr<Gui>;

   static void create()
   {
      instance = gui_ptr_type{ new Gui{} };
   }
   
   static gui_ptr_type instance;
};

inline Gui::gui_ptr_type Gui::instance = gui_ptr_type{nullptr};

} /* namespace Graphics */

#endif /* QUEST_GRAPHICS_GUI_HPP_INCLUDED */
