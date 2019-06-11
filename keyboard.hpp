#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include <thread>
#include <future>
#include <mutex>
#include <iostream>
#include <fcntl.h> // open
#include <unistd.h> // close
#include <linux/input.h> // struct input_event

#include "util/Singleton.hpp"
#include "event_handler.hpp"
#include "blocking_queue.hpp"

//class keyboard_queue;
//
///*
// * struct keyboard_state - represents keyboards current state
// */
//class keyboard_state
//   : public Singleton<keyboard_state>
//{
//   private:
//      mutable std::mutex m_keystate_mutex;
//      signed short m_keystate_current[KEY_CNT];
//      signed short m_keystate_prev[KEY_CNT];
//      
//      /**
//       * update keyboard state
//       **/
//      void update(input_event* keyboard_event)
//      {
//         std::lock_guard<std::mutex> keystate_lock(m_keystate_mutex);
//         m_keystate_prev[keyboard_event->code] = m_keystate_current[keyboard_event->code];
//         m_keystate_current[keyboard_event->code] = keyboard_event->value;
//      }
//
//      friend keyboard_queue;
//   
//   public:
//      short signed keystate_current(unsigned short key) const 
//      {
//         std::lock_guard<std::mutex> keystate_lock(m_keystate_mutex);
//         return m_keystate_current[key];
//      }
//};
//
//std::ostream& operator<<(std::ostream& os, const keyboard_state& state)
//{
//   for(int i = 0; i < KEY_CNT; ++i)
//   {
//      os << state.keystate_current(i);
//   }
//   return os;
//}

/*
 * struct keyboard_queue - represents keyboard event queue
 */
class keyboard_queue
   //: public Singleton<keyboard_queue>
   : public event_handler<char, void(const char&)>
   , private blocking_queue<char>
{
   public:
   using input_event_t     = char;

   using event_handler_t   = event_handler<char, void(const char&)>;
   using queue_t           = blocking_queue<char>;

   using event_t           = typename event_handler_t::event_t;
   using function_t        = typename event_handler_t::function_t;

   private:
      //keyboard_state& m_keyboard_state = keyboard_state::instance();

   public:
      void update(const input_event_t& keyboard_event)
      {
         queue_t::push(keyboard_event);
      }

      void register_event(const event_t& event, const function_t& f)
      {
         event_handler_t::register_function(event, f);
      }

      void handle_events()
      {
         while(!queue_t::empty())
         {
            // get an event
            input_event_t event;
            queue_t::pop_try_wait(event); // 
            //m_keyboard_state.update(&event); // first update keyboard state
            
            // then handle event
            event_handler_t::handle_event(event, event);
         }
      }
};

/*
 *
 */
class keyboard
   :  public Util::Singleton<keyboard>
   ,  public keyboard_queue
{
   private:
      bool m_active      = false;
      //std::unique_ptr<input_event> m_keyboard_event = std::unique_ptr<input_event>(new input_event);
      //keyboard_queue& m_keyboard_queue = keyboard_queue::instance();
      std::future<void> m_return_from_thread;
      
      bool is_active() const
      {
         return m_active;
      }
      
      //void read_event()
      //{
      //   //int bytes = read(m_keyboard_fd, m_keyboard_event.get(), sizeof(*m_keyboard_event));
      //   char ch = getch();
      //   if(ch)
      //   {
      //      //std::cout << "code : " << m_keyboard_event->code << std::endl;
      //      //std::cout << "value: " << m_keyboard_event->value << std::endl;
      //      //std::cout << std::endl;
      //      //if(m_keyboard_event->type & EV_KEY) // check that it is actally a keyboard event
      //      //{
      //         //m_keyboard_state.update(m_keyboard_event.get());
      //      keyboard_queue::update(ch);
      //      //}
      //   }
      //}
      
      void loop()
      {
         while(is_active())
         {
            read_event();
         }
      }

   public:
      keyboard()
      {
         timeout(0);
         //m_active = true;
         //m_return_from_thread = std::async(std::launch::async,std::bind(&keyboard::loop, this));
      }

      keyboard(const keyboard&) = delete;
      keyboard& operator=(const keyboard&) = delete;
      
      ~keyboard()
      {
         //m_active = false;
         //m_return_from_thread.wait(); // wait for thread to finish
      }
      
      void read_event()
      {
         //int bytes = read(m_keyboard_fd, m_keyboard_event.get(), sizeof(*m_keyboard_event));
         char ch = getch();
         if(ch != ERR)
         {
            //std::cout << "code : " << m_keyboard_event->code << std::endl;
            //std::cout << "value: " << m_keyboard_event->value << std::endl;
            //std::cout << std::endl;
            //if(m_keyboard_event->type & EV_KEY) // check that it is actally a keyboard event
            //{
               //m_keyboard_state.update(m_keyboard_event.get());
            keyboard_queue::update(ch);
            //}
         }
      }
};

#endif /* KEYBOARD_H_INCLUDED */
