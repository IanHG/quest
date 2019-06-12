#pragma once
#ifndef QUEST_GAME_DIALOG_HPP_INCLUDED
#define QUEST_GAME_DIALOG_HPP_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

namespace Game
{

struct DialogPiece;

struct DialogChoice
{
   using response_type = std::string;
   using next_type     = DialogPiece*;
   
   response_type response;
   next_type     next = nullptr;

   DialogChoice(const std::string& str)
      :  response(str)
   {
   }

   void draw()
   {
      std::cout << response << std::endl;
   }
};

struct DialogPiece
{
   using dialog_type  = std::string;
   using choices_type = std::vector<DialogChoice>;
   
   dialog_type  dialog;
   choices_type choices;

   void draw()
   {
      std::cout << dialog << std::endl;
      for(auto& choice : choices)
      {
         choice.draw();
      }
   }
};

/**
 * Container for dialogues.
 **/
struct DialogContainer
{
   using container_type = std::vector<DialogPiece>;

   container_type container;

   void load(const std::string& dialog_name) //, const std::string& dialog_path)
   {
      std::ifstream dialog_file{"dialogs/" + dialog_name};

      auto create_dialog_piece= [](std::ifstream& ifs, std::string& str){
         DialogPiece dialog_piece;

         while(std::getline(ifs, str))
         {
            
         }
      };

      std::string str;
      while(std::getline(dialog_file, str))
      {

      }
   }

   using dialog_container_ptr = std::unique_ptr<DialogContainer>;

   static dialog_container_ptr instance;
};

/**
 * Interface. Handles dialogues in game.
 **/
struct Dialog
{
   using next_type = DialogPiece*;
   using tag_type  = std::string;

   next_type next;

   void draw()
   {
      if(next)
      {
         next->draw();
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
      Dialog dialog;
      dialog.next = new DialogPiece{"Hello!"};
      dialog.next->choices.emplace_back(std::string("lol"));

      return dialog;
   }
};

} /* namespace Game */

#endif /* QUEST_GAME_DIALOG_HPP_INCLUDED */
