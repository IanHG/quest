#include "Console.hpp"

#include <stdlib.h>
#include <iostream>

#include "../graphics/TypeDefs.hpp"
#include "../graphics/Gui.hpp"

#include "../util/String.hpp"

namespace Engine
{

ChatMessage* addConsoleMessageFromEditor(Console& console)
{
   char* message = (char*)malloc(1024);
   console.editor->str.get(message, 1024);
   ChatMessage* chat_message = nullptr;
   if(console.editor->str.current_size > 0)
   {
      chat_message = console.addChatMessage(message, console.editor->str.current_size);
   }
   console.editor->str.clear(); 

   return chat_message;
}

void executeConsoleCommand(ChatMessage& message)
{
   auto loadReply = [](ChatMessage& reply, const std::string& reply_str){
      memcpy(reply.reply, reply_str.c_str(), reply_str.size());
      reply.reply[reply_str.size()] = '\0';
      reply.reply_size = reply_str.size();
   };

   auto console_command = Util::splitString(message.message);

   if(!message.reply)
   {
      message.reply = (char*)malloc(1024 * sizeof(char));
   }

   if(console_command[0] == "echo")
   {
      std::string reply_str = "'";
      for(int i = 1; i < console_command.size(); ++i)
      {
         reply_str += console_command[i];
      }
      reply_str += "'";
      loadReply(message, reply_str);
   }
   else
   {
      std::string reply_str = "Unknown command '" + console_command[0] + "'.";
      loadReply(message, reply_str);
   }

   Graphics::Gui::instance->message("command executed");

   message.executed = true;
}

/**
 *
 **/
Console::Console()
{
   window_x_offset = 0;
   window_y_offset = 0;
   window_x_size   = 150;
   window_y_size   = open_t;
   
   // Enter command
   mer.addEvent(KEY_ENTER, [this](){ 
      ChatMessage* chat_message = addConsoleMessageFromEditor(*this);
      if(chat_message)
      {
         executeConsoleCommand(*chat_message);
      }
   } );
   mer.addEvent(10 , [this](){ 
      ChatMessage* chat_message = addConsoleMessageFromEditor(*this);
      if(chat_message)
      {
         executeConsoleCommand(*chat_message);
      }
   } );

   // key up
   mer.addEvent(259, [this](){ 
      // if market == current, save editor string in current chat sequence
      if(marked_chat_sequence == current_chat_sequence)
      {
         addConsoleMessageFromEditor(*this);
      }
      int new_marked_chat_sequence = std::max(0, (marked_chat_sequence - 1));
      if(chat_sequence[new_marked_chat_sequence].size)
      {
         editor->str.set(chat_sequence[new_marked_chat_sequence].message, chat_sequence[new_marked_chat_sequence].size);
         marked_chat_sequence = std::max(0, (marked_chat_sequence - 1));
      }
   } );
   // key down
   mer.addEvent(258, [this](){ 
      int new_marked_chat_sequence = (marked_chat_sequence + 1) % chat_sequence_size;
      if(new_marked_chat_sequence <= current_chat_sequence)
      {
         editor->str.set(chat_sequence[new_marked_chat_sequence].message, chat_sequence[new_marked_chat_sequence].size);
         marked_chat_sequence = (marked_chat_sequence + 1) % chat_sequence_size;
      }
   } );
}

/**
 *
 **/
ChatMessage* Console::addChatMessage(const char* message, int size)
{
   if(chat_sequence[current_chat_sequence].message) 
   {
      free(chat_sequence[current_chat_sequence].message);
   }
   chat_sequence[current_chat_sequence].message  = (char*)message;
   chat_sequence[current_chat_sequence].size     = size;

   current_chat_sequence = (current_chat_sequence + 1) % chat_sequence_size;
   marked_chat_sequence  = current_chat_sequence;

   return &chat_sequence[current_chat_sequence - 1];
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
      for(int y = window_y_offset; y < window_y_max - 1; ++y)
      {
         Graphics::drawCell(stdscr, x, y, ' ', 1);
      }
   }
   for(int x = window_x_offset; x < window_x_max; ++x)
   {
      for(int y = window_y_max - 1; y < window_y_max; ++y)
      {
         Graphics::drawCell(stdscr, x, y, ' ', 5);
      }
   }
   
   int current = current_chat_sequence - 1;
   for(int line = window_y_max - 2; line >= 0; --line)
   {
      if(chat_sequence[current].executed)
      {
         for(int i = 0; i < chat_sequence[current].size; ++i)
         {
            Graphics::drawCell(stdscr, i, line, chat_sequence[current].message[i], 1);
         }
         int reply_start = chat_sequence[current].size + 1;
         for(int i = 0; i < chat_sequence[current].reply_size; ++i)
         {
            Graphics::drawCell(stdscr, reply_start + i, line, chat_sequence[current].reply[i], 1);
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
         Graphics::drawCell(stdscr, i, window_y_max - 1, editor->str.str[i], 5);
      }
      
      Graphics::drawCell(stdscr, editor->str.position, window_y_max - 1
            , editor->str.position < editor->str.current_size ? editor->str.str[editor->str.position] : ' '
            , 2);
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

   updateOpenness();
}

} /* namespace Engine */
