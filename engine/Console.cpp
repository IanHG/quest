#include "Console.hpp"

#include <stdlib.h>
#include <iostream>

#include "../graphics/TypeDefs.hpp"
#include "../graphics/Gui.hpp"

namespace Engine
{

/**
 *
 **/
Console::Console()
{
   window_x_offset = 0;
   window_y_offset = 0;
   window_x_size   = 70;
   window_y_size   = open_t;
   
   mer.addEvent(KEY_ENTER, [this](){ 
      char* message = (char*)malloc(1024);
      editor->str.get(message, 1024);
      addChatMessage(message, editor->str.current_size);
      editor->str.clear(); 
   } );
   mer.addEvent(10 , [this](){ 
      char* message = (char*)malloc(1024);
      editor->str.get(message, 1024);
      addChatMessage(message, editor->str.current_size);
      editor->str.clear(); 
   } );

   //// key up
   //mer.addEvent(259, [this](){ 
   //   marked_chat_sequence = std::min(0, marked_chat_sequence - 1);
   //   editor->str.set(chat_sequence[marked_chat_sequence].message, chat_sequence[marked_chat_sequence].size);
   //} );
   //// key down
   //mer.addEvent(258, [this](){ 
   //   marked_chat_sequence = (marked_chat_sequence + 1) % chat_sequence_size;
   //   editor->str.set(chat_sequence[marked_chat_sequence].message, chat_sequence[marked_chat_sequence].size);
   //} );
}

/**
 *
 **/
ChatMessage* Console::addChatMessage(const char* message, int size)
{
   current_chat_sequence = (current_chat_sequence + 1) % chat_sequence_size;
   if(chat_sequence[current_chat_sequence].message) 
   {
      free(chat_sequence[current_chat_sequence].message);
   }
   chat_sequence[current_chat_sequence].message = (char*)message;
   chat_sequence[current_chat_sequence].size    = size;

   marked_chat_sequence = current_chat_sequence;

   return &chat_sequence[current_chat_sequence];
}

/**
 *
 **/
void Console::draw()
{
   updateOpenness();
   
   if(!amIOpen())
   {
      return;
   }

   int window_x_max = window_x_offset + window_x_size;
   int window_y_max = window_y_offset + window_y_size;

   // Draw rectangle
   for(int x = window_x_offset; x < window_x_max; ++x)
   {
      for(int y = window_y_offset; y < window_y_max; ++y)
      {
         Graphics::drawCell(stdscr, x, y, ' ', 1);
      }
   }
   
   int current = current_chat_sequence;
   for(int line = window_y_max - 2; line >= 0; --line)
   {
      if(chat_sequence[current].message)
      {
         for(int i = 0; i < chat_sequence[current].size; ++i)
         {
            Graphics::drawCell(stdscr, i, line, chat_sequence[current].message[i], 1);
         }
      }
      --current;
      if(current < 0)
      {
         current = chat_sequence_size - 1;
      }
   }

   //
   if(window_y_size > 0)
   {
      for(int i = 0; i < editor->str.current_size; ++i)
      {
         Graphics::drawCell(stdscr, i, window_y_max - 1, editor->str.str[i], 1);
      }

   }
}

/**
 *
 **/
bool Console::amIOpen() const
{
   return (open_t != open_type(0));
}

/**
 *
 **/
void Console::updateOpenness()
{
   if(open_t != open_t_target)
   {
      if(open_t < open_t_target)
      {
         open_t += dopen_dt;
      }
      else
      {
         open_t -= dopen_dt;
      }
   }

   window_y_size = open_t;
}

/**
 *
 **/
bool Console::openOrClose(open_type extent)
{
   if(open_t_target > 0)
   {
      open_t_target = 0;
      mer.deregisterEvents(Engine::Keyboard::instance());
      return false;
   }
   else
   {
      open_t_target = extent;
      mer.registerEvents(Engine::Keyboard::instance());
      return true;
   }
}

} /* namespace Engine */
