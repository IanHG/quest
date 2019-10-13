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
      Graphics::Gui::instance->message("insert: " + std::to_string(c) + "\n");
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

   void del(bool inplace = false)
   {
      Graphics::Gui::instance->message("deleting\n");
      if(position != current_size)
      {
         for(int i = inplace ? position + 1 : position; i < current_size; ++i)
         {
            str[i - 1] = str[i];
         }
      }

      if(position > 0 && (!inplace || position == current_size))
      {
         --position;
      }
      if(current_size > 0)
      {
         --current_size;
      }
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
      current_size = n_copy;
      position     = n_copy;
   }
};

struct Editor
{
   enum Events : int { UTILITY, ALPHA, NUMERIC, SPECIAL, ALL };

   MultiEventRegisterer<event_handler<ichtype, void()> > mer_utility;
   MultiEventRegisterer<event_handler<ichtype, void()> > mer_alpha;
   MultiEventRegisterer<event_handler<ichtype, void()> > mer_numeric;
   MultiEventRegisterer<event_handler<ichtype, void()> > mer_special;

   EditorString str;

   bool is_active = false;
   
   Editor()
   {
      // Alphanumeric keys
      #define generate_alpha_event(C) \
         mer_alpha.addEvent(C     , [this](){ str.insert(C); } ); \
         mer_alpha.addEvent(C + 32, [this](){ str.insert(C + 32); } );

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
      
      #undef generate_alpha_event
      
      #define generate_special_event(S) \
         mer_special.addEvent(S, [this](){ str.insert(S); } );
      
      generate_special_event('.');
      generate_special_event(':');
      generate_special_event(',');
      generate_special_event(';');
      generate_special_event('?');
      generate_special_event('!');

      
      #define generate_numeric_event(N) \
         mer_numeric.addEvent(N, [this](){ str.insert(N); } );

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
      mer_utility.addEvent(' ', [this](){ str.insert(' '); } );

      // Arrow keys
      //mer_utility.addEvent(KEY_LEFT  , [this](){ str.decrPosition(); } );
      mer_utility.addEvent(260       , [this](){ str.decrPosition(); } );
      //mer_utility.addEvent(KEY_RIGHT , [this](){ str.incrPosition(); } );
      mer_utility.addEvent(261       , [this](){ str.incrPosition(); } );

      // Special keys
      //mer_utility.addEvent(KEY_ENTER    , [this](){ str.clear(); } );
      //mer_utility.addEvent(10           , [this](){ str.clear(); } );
      mer_utility.addEvent(KEY_DL       , [this](){ str.del(); } );
      mer_utility.addEvent(KEY_BACKSPACE, [this](){ str.del(); } );
      mer_utility.addEvent(263          , [this](){ str.del(); } );
      
      // Delete inplace
      mer_utility.addEvent(330, [this](){ str.del(true); } );

      //#define KEY_ESC 27

      //event_registerable<keyboard>::register_function(kb, KEY_ESC , [this, &kb](){ 
      //   this->deregister_events(kb);
      //   event_registerable<keyboard>::register_function(kb, KEY_ESC, [this, &kb](){ this->register_events(kb); } );
      //} );

      //#undef KEY_ESC
   }

   void enable(Events events)
   {
      switch(events)
      {
         case UTILITY:
            mer_utility.registerEvents(Engine::Keyboard::instance());
            break;
         case ALPHA:
            mer_alpha.registerEvents(Engine::Keyboard::instance());
            break;
         case NUMERIC:
            mer_numeric.registerEvents(Engine::Keyboard::instance());
            break;
         case SPECIAL:
            mer_special.registerEvents(Engine::Keyboard::instance());
            break;
         case ALL:
            mer_utility.registerEvents(Engine::Keyboard::instance());
            mer_alpha.registerEvents(Engine::Keyboard::instance());
            mer_numeric.registerEvents(Engine::Keyboard::instance());
            mer_special.registerEvents(Engine::Keyboard::instance());
            break;
      }
   }

   void disable(Events events)
   {
      switch(events)
      {
         case UTILITY:
            mer_utility.deregisterEvents(Engine::Keyboard::instance());
            break;
         case ALPHA:
            mer_alpha.deregisterEvents(Engine::Keyboard::instance());
            break;
         case NUMERIC:
            mer_numeric.deregisterEvents(Engine::Keyboard::instance());
            break;
         case SPECIAL:
            mer_special.deregisterEvents(Engine::Keyboard::instance());
            break;
         case ALL:
            mer_utility.deregisterEvents(Engine::Keyboard::instance());
            mer_alpha.deregisterEvents(Engine::Keyboard::instance());
            mer_numeric.deregisterEvents(Engine::Keyboard::instance());
            mer_special.deregisterEvents(Engine::Keyboard::instance());
            break;
      }
   }
};

} /* namespace Engine */

#endif /* QUEST_ENGINE_EDITOR_HPP_INCLUDED */
