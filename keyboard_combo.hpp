#pragma once
#ifndef KEYBOARD_COMBO_HPP_INCLUDED
#define KEYBOARD_COMBO_HPP_INCLUDED

#include <chrono>
#include <vector>

#include "keyboard.hpp"
#include "event_handler.hpp"

/**
 * Keyboard combo for cheat codes
 **/
struct keyboard_combo
{
   using function_t = std::function<void()>;
   
   keyboard&                  m_keyboard;
   const function_t           m_function;
   const std::vector<char>    m_combo;
   const std::chrono::seconds m_timespan = std::chrono::seconds{1};
   std::chrono::time_point<std::chrono::system_clock> last_hit;
   std::vector<event_registerer<typename keyboard::event_handler_t> > m_register;

   auto timespan_ok()
   {
      auto now = std::chrono::system_clock::now();
      auto diff = now - this->last_hit;
      this->last_hit = now;
      return diff < m_timespan;
   }
   
   keyboard_combo(const std::vector<char>& combo, const function_t& func, keyboard& kb)
      :  m_keyboard(kb)
      ,  m_function(func)
      ,  m_combo(combo)
      ,  m_register(m_combo.size())
   {
      m_register[0].set_event(m_combo[0], [this](const char&){
         auto now = std::chrono::system_clock::now();
         auto diff = now - this->last_hit;
         if(diff >= m_timespan)
         {
            this->last_hit = std::chrono::system_clock::now();
            m_register[1].register_event  (m_keyboard);
            
            for(std::size_t i = 2; i < m_register.size(); ++i)
            {
               m_register[i].deregister_event(m_keyboard);
            }
         }
      });

      for(std::size_t i = 1; i < m_combo.size() - 1; ++i)
      {
         m_register[i].set_event(m_combo[i], [this, i](const char&){
            if(this->timespan_ok())
            {
               m_register[i + 1].register_event(m_keyboard);
            }
            m_register[i].deregister_event(m_keyboard);
         });
      }
      
      auto last = this->m_combo.size() - 1;
      m_register[last].set_event(this->m_combo[last], [this, last](const char&){
         if(this->timespan_ok())
         {
            this->m_function();
         }
         
         m_register[last].deregister_event(m_keyboard);
      });

      m_register[0].register_event(m_keyboard);
   }
};

#endif /* KEYBOARD_COMBO_HPP_INCLUDED */
