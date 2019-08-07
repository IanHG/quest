#pragma once
#ifndef QUEST_ENGINE_EDITOR_HPP_INCLUDED
#define QUEST_ENGINE_EDITOR_HPP_INCLUDED

#include <cstring>

#include "Keyboard.hpp"
#include "../graphics/Gui.hpp"

namespace Engine
{

struct EditorString
{
   static constexpr int str_size = 1024;
   char             str[str_size];
   int              position = 0;
   int              current_size = 0;

   EditorString()
   {
      clear();
   }

   void insert(char c)
   {
      if(position != current_size)
      {
         for(int i = current_size; i > position; --i)
         {
            str[i] = str[i - 1];
         }
      }
      str[position] = c;
      ++current_size;
      ++position;
   }

   void del()
   {
      Graphics::Gui::instance->message("deleting\n");
      if(position != current_size)
      {
         for(int i = position; i < current_size; ++i)
         {
            str[i - 1] = str[i];
         }
      }
      --current_size;
      --position;
   }

   void incrPosition()
   {
      if(position < current_size)
      {
         ++position;
      }
   }

   void decrPosition()
   {
      if(position > 0)
      {
         --position;
      }
   }
   
   void clear()
   {
      for(int i = 0; i < str_size; ++i)
      {
         str[i] = ' ';
      }
      position      = 0;
      current_size  = 0;
      str[position] = '\0';
   }

   void get(char* ptr, int size)
   {
      int n_copy = std::min(size, current_size);
      memcpy(ptr, str, n_copy);
      ptr[n_copy] = '\0';
   }

   void set(char* ptr, int size)
   {
      int n_copy = std::min(size, str_size);
      memcpy(str, ptr, n_copy);
      str[n_copy] = '\0';
   }
};

struct Editor
{
   MultiEventRegisterer<event_handler<ichtype, void()> > mer;

   EditorString str;

   bool is_active = false;
   
   Editor()
   {
      // Alphanumeric keys
      #define generate_alpha_event(C) \
         mer.addEvent(C     , [this](){ str.insert(C); } ); \
         mer.addEvent(C + 32, [this](){ str.insert(C + 32); } );

      generate_alpha_event('A');
      generate_alpha_event('B');
      generate_alpha_event('C');
      generate_alpha_event('D');
      generate_alpha_event('E');
      generate_alpha_event('F');
      generate_alpha_event('G');
      generate_alpha_event('H');
      generate_alpha_event('I');
      generate_alpha_event('J');
      generate_alpha_event('K');
      generate_alpha_event('L');
      generate_alpha_event('M');
      generate_alpha_event('N');
      generate_alpha_event('O');
      generate_alpha_event('P');
      generate_alpha_event('Q');
      generate_alpha_event('R');
      generate_alpha_event('S');
      generate_alpha_event('T');
      generate_alpha_event('U');
      generate_alpha_event('V');
      generate_alpha_event('W');
      generate_alpha_event('X');
      generate_alpha_event('Y');
      generate_alpha_event('Z');
      
      generate_alpha_event('.');
      generate_alpha_event(':');
      generate_alpha_event(',');
      generate_alpha_event(';');
      generate_alpha_event('?');
      generate_alpha_event('!');

      #undef generate_alpha_event
      
      #define generate_numeric_event(N) \
         mer.addEvent(N, [this](){ str.insert(N); } );

      generate_numeric_event('1');
      generate_numeric_event('2');
      generate_numeric_event('3');
      generate_numeric_event('4');
      generate_numeric_event('5');
      generate_numeric_event('6');
      generate_numeric_event('7');
      generate_numeric_event('8');
      generate_numeric_event('9');
      generate_numeric_event('0');

      #undef generate_numeric_event
      
      // Space
      mer.addEvent(' ', [this](){ str.insert(' '); } );

      // Arrow keys
      mer.addEvent(KEY_LEFT  , [this](){ str.decrPosition(); } );
      mer.addEvent(260       , [this](){ str.decrPosition(); } );
      mer.addEvent(KEY_RIGHT , [this](){ str.incrPosition(); } );
      mer.addEvent(261       , [this](){ str.incrPosition(); } );

      // Special keys
      //mer.addEvent(KEY_ENTER    , [this](){ str.clear(); } );
      //mer.addEvent(10           , [this](){ str.clear(); } );
      mer.addEvent(KEY_DL       , [this](){ Graphics::Gui::instance->message("DELTE\n"); str.del(); } );
      mer.addEvent(KEY_BACKSPACE, [this](){ Graphics::Gui::instance->message("DELTE\n"); str.del(); } );
      mer.addEvent(263, [this](){ Graphics::Gui::instance->message("DELTE\n"); str.del(); } );

      //#define KEY_ESC 27

      //event_registerable<keyboard>::register_function(kb, KEY_ESC , [this, &kb](){ 
      //   this->deregister_events(kb);
      //   event_registerable<keyboard>::register_function(kb, KEY_ESC, [this, &kb](){ this->register_events(kb); } );
      //} );

      //#undef KEY_ESC
   }

   void enable()
   {
      mer.registerEvents(Engine::Keyboard::instance());
   }

   void disable()
   {
      mer.deregisterEvents(Engine::Keyboard::instance());
   }
};

} /* namespace Engine */

#endif /* QUEST_ENGINE_EDITOR_HPP_INCLUDED */
