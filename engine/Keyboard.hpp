#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include <thread>
#include <future>
#include <mutex>
#include <iostream>
#include <fcntl.h> // open
#include <unistd.h> // close
#include <linux/input.h> // struct input_event
#include <chrono>
#include <vector>

#include <curses.h>

#include "../util/Singleton.hpp"
#include "../event_handler.hpp"
#include "../blocking_queue.hpp"
#include "../graphics/Gui.hpp"

namespace Engine
{

using ichtype = int;

/*
 * struct keyboard_queue - represents keyboard event queue
 */
class KeyboardQueue
   : public  event_handler <ichtype, void()>
   , private blocking_queue<ichtype>
{
   public:
   using input_event_t     = ichtype;

   using event_handler_t   = event_handler <ichtype, void()>;
   using queue_t           = blocking_queue<ichtype>;

   using event_t           = typename event_handler_t::event_t;
   using function_t        = typename event_handler_t::function_t;

   public:
      void update(const input_event_t& keyboard_event)
      {
         queue_t::push(keyboard_event);
      }

      void registerEvent(const event_t& event, const function_t& f)
      {
         event_handler_t::register_function(event, f);
      }

      void handleEvents()
      {
         while(!queue_t::empty())
         {
            // get an event
            input_event_t event;
            queue_t::pop_try_wait(event); // 
            
            // then handle event
            event_handler_t::handle_event(event);
         }
      }
};

/*
 *
 */
class Keyboard
   :  public Util::Singleton<Keyboard>
   ,  public KeyboardQueue
{
   public:
      Keyboard()
      {
      }

      Keyboard(const Keyboard&)            = delete;
      Keyboard& operator=(const Keyboard&) = delete;
      
      ~Keyboard()
      {
      }
      
      // Read keyboard event
      void readEvent()
      {
         ichtype ch = getch();
         if(ch != ERR)
         {
            Graphics::Gui::instance->message(std::to_string(ch));
            KeyboardQueue::update(ch);
         }
      }

      // Read keyboard events
      void readEvents()
      {
         ichtype ch;
         while((ch = getch()) != ERR)
         {
            Graphics::Gui::instance->message(std::to_string(ch) + " ");
            KeyboardQueue::update(ch);
         }
      }
};


/**
 * Keyboard combo for cheat codes
 **/
struct KeyboardCombo
{
   using function_t = std::function<void()>;
   
   Keyboard&                  m_keyboard;
   const function_t           m_function;
   const std::vector<ichtype> m_combo;
   const std::chrono::seconds m_timespan = std::chrono::seconds{1};
   std::chrono::time_point<std::chrono::system_clock> last_hit;
   std::vector<EventRegisterer<typename Keyboard::event_handler_t> > m_register;

   auto timespan_ok()
   {
      auto now = std::chrono::system_clock::now();
      auto diff = now - this->last_hit;
      this->last_hit = now;
      return diff < m_timespan;
   }
   
   KeyboardCombo(const std::vector<ichtype>& combo, const function_t& func, Keyboard& kb)
      :  m_keyboard(kb)
      ,  m_function(func)
      ,  m_combo(combo)
      ,  m_register(m_combo.size())
   {
      m_register[0].setEvent(m_combo[0], [this](){
         auto now = std::chrono::system_clock::now();
         auto diff = now - this->last_hit;
         if(diff >= m_timespan)
         {
            this->last_hit = std::chrono::system_clock::now();
            m_register[1].registerEvent  (m_keyboard);
            
            for(std::size_t i = 2; i < m_register.size(); ++i)
            {
               m_register[i].deregisterEvent(m_keyboard);
            }
         }
      });

      for(std::size_t i = 1; i < m_combo.size() - 1; ++i)
      {
         m_register[i].setEvent(m_combo[i], [this, i](){
            if(this->timespan_ok())
            {
               m_register[i + 1].registerEvent(m_keyboard);
            }
            m_register[i].deregisterEvent(m_keyboard);
         });
      }
      
      auto last = this->m_combo.size() - 1;
      m_register[last].setEvent(this->m_combo[last], [this, last](){
         if(this->timespan_ok())
         {
            this->m_function();
         }
         
         m_register[last].deregisterEvent(m_keyboard);
      });
   }

   void enable()
   {
      m_register[0].registerEvent(m_keyboard);
   }

   void disable()
   {
      m_register[0].deregisterEvent(m_keyboard);
   }
};

//
using KeyboardRegisterer = MultiEventRegisterer<event_handler<ichtype, void()> >;

} /* namespace Engine */

#endif /* KEYBOARD_H_INCLUDED */
