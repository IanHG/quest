#pragma once
#ifndef QUEST_GAME_DIALOG_HPP_INCLUDED
#define QUEST_GAME_DIALOG_HPP_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>

#include "../graphics/TypeDefs.hpp"
#include "../graphics/Gui.hpp"

namespace Game
{

struct DialogPiece;

struct DialogChoice
{
   using response_type = std::string;
   using next_type     = DialogPiece*;
   
   response_type response;
   next_type     next = nullptr;
};

struct DialogPiece
{
   using dialog_type  = std::string;
   using choices_type = std::vector<DialogChoice>;
   
   dialog_type  dialog;
   choices_type choices;

   void draw(WINDOW* win)
   {
      werase(win);
      wprintw(win, dialog.c_str());
      wprintw(win, "\n");
      wprintw(win, "\n");
      for(decltype(choices.size()) i = 0; i < choices.size(); ++i)
      {
         wprintw(win, "%i. ", i + 1);
         wprintw(win, choices[i].response.c_str());
         wprintw(win, "\n");

      }
   }
};

struct DialogStruct;

/**
 * Container for dialogues.
 **/
struct DialogContainer
{
   
   using container_type = std::vector<DialogPiece>;

   container_type container;

   void load(const std::string& dialog_name); //, const std::string& dialog_path)

   using dialog_container_ptr = std::unique_ptr<DialogContainer>;

   static dialog_container_ptr instance;
};

inline DialogContainer::dialog_container_ptr DialogContainer::instance = dialog_container_ptr{ new DialogContainer{} };

/**
 * Interface. Handles dialogues in game.
 **/
struct Dialog
{
   using next_type = DialogPiece*;
   using tag_type  = std::string;

   next_type next;
   Graphics::Gui::WindowIndex window_index = Graphics::Gui::WindowIndex{0};

   Dialog()
   {
      auto main_window   = Graphics::Gui::getWindow(Graphics::Gui::Window::MAIN);
      this->window_index = Graphics::Gui::createWindow
         (  Graphics::Gui::Window::MAIN
         ,  main_window->xMax()
         ,  main_window->yMax() / 2
         ,  0
         ,  0
         );
   }

   ~Dialog()
   {
      Graphics::Gui::destroyWindow(this->window_index);
   }

   void draw()
   {
      if(next)
      {
         auto window = Graphics::Gui::getWindow(window_index);
         if(window)
         {
            next->draw(window->getWindow());
         }
      }
   }

   void option(int i)
   {
      i = i - 1;
      if((i >= 0) && (i < int(next->choices.size())))
      {
         if(next)
         {
            next = next->choices[i].next;
         }
      }
   }

   bool ended() const
   {
      return bool(next);
   }

   static Dialog load(const tag_type& tag)
   {
      DialogContainer::instance->load("oracle.dialog");
      
      Dialog dialog;

      dialog.next = &(DialogContainer::instance->container[0]);

      return dialog;
   }
};

} /* namespace Game */

#endif /* QUEST_GAME_DIALOG_HPP_INCLUDED */
