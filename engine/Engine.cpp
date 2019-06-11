#include "Engine.hpp"

#include <chrono>
#include <string>
#include <sstream>
#include <thread>
#include <curses.h>

#include "../graphics/Gui.hpp"
#include "Keyboard.hpp"

namespace Engine
{

struct Frame
{
   decltype(std::chrono::milliseconds(int(1)/int(1))) frame      = std::chrono::milliseconds(1000 / 144);
   decltype(std::chrono::system_clock::now())         next_frame = std::chrono::system_clock::now();
   decltype(next_frame - frame)                       last_frame = next_frame - frame;
   
   void waitForNextFrame()
   {
      // Wait for next frame
      last_frame = next_frame;
      next_frame += frame;
      std::this_thread::sleep_until(next_frame);
   }

   std::string showFrameRate() const
   {      
      std::stringstream s_str;
      s_str << "Time: "  // just for monitoring purposes
            << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - last_frame).count()
            << "ms\n";
      return s_str.str();
   }
} frame;


/**
 *
 **/
void initialize()
{
   // No time out on read
   timeout(0);

   Keyboard& kb = Keyboard::instance();
   kb.handle_noevent();
}

/**
 *
 **/
void gameLoop(const std::function<bool()>& function)
{
   Keyboard& kb = Keyboard::instance();
   kb.handle_noevent();

   while (function())
   {
      // Handle keyboard events
      kb.readEvents();
      kb.handleEvents();
      
      // Refresh gui
      Graphics::Gui::instance->refresh();
      
      // Wait for next frame
      frame.waitForNextFrame();
   }
}

} /* namespace Engine */
