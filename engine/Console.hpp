#pragma once
#ifndef QUEST_ENGINE_CONSOLE_HPP_INCLUDED
#define QUEST_ENGINE_CONSOLE_HPP_INCLUDED

#include <stdlib.h>

#include "Editor.hpp"

namespace Engine
{

//struct WindowBorder
//{
//   char ul_corner = '+';
//   char ur_corner = '+';
//   char bl_corner = '+';
//   char br_corner = '+';
//
//   char vertical   = '|';
//   char horizontal = '-';
//};

/**
 *
 **/
struct ChatMessage
{
   char* message  = nullptr;
   int   size     = 0;
   
   char* reply      = nullptr;
   int   reply_size = 0;
   bool  executed   = false;

   ~ChatMessage()
   {
      if(message)
      {
         free(message);
      }
      if(reply)
      {
         free(reply);
      }
   }
};

/**
 * Notes:
 *    Type Commands and get reply
 *    Autocompletion
 *    Scrollbar
 *    History of old commands
 **/
struct Console
{
   using open_type = int;
   enum : open_type {
      BIG    = 20,
      SMALL  = 5,
      CLOSED = 0,
   };

   //
   static constexpr int chat_sequence_size  = 100;
   ChatMessage   chat_sequence[chat_sequence_size];
   int           current_chat_sequence = 0;
   int           marked_chat_sequence  = 0;

   Editor* editor;
   MultiEventRegisterer<event_handler<ichtype, void()> > mer;

   //
   open_type open_t = 0;        /* openness now */
   open_type open_t_target = 0; /* openness target */
   open_type open_t_max = 70;   /* openness max */
   open_type dopen_dt = 1;      /* delta openness (rate of openning) */

   // window stuff
   int window_x_offset;
   int window_y_offset;
   int window_x_size;
   int window_y_size;

   Console();
   
   ChatMessage* addChatMessage(const char* message, int size);

   void draw();
   bool amIOpen() const;
   void updateOpenness();

   bool openOrClose(open_type extent);
};

/**
 *
 **/
struct ConsoleCommands
{
   using command_map_type = std::map<const char[], void(const char*[], int)>;

   command_map_type commands;
};

} /* namespace Engine */

#endif /* QUEST_ENGINE_CONSOLE_HPP_INCLUDED */
