#include "Map.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "../graphics/Gui.hpp"

namespace Game
{

/**
 * Load map file and return map struct.
 **/
Map Map::load(const std::string& map_name)
{
   Map map;

   std::ifstream map_file{"maps/" + map_name};
   std::string str;
   std::getline(map_file, str);
   int max_line, max_col;
   std::istringstream s_str(str);
   s_str >> max_line >> max_col;
   
   map.x_size = max_col;
   map.y_size = max_line;
   
   auto window = Graphics::Gui::getWindow(Graphics::Gui::Window::MAIN);

   map.x_offset = window->xMax() / 2 - map.x_size / 2;
   map.y_offset = window->yMax() / 2 - map.y_size / 2;

   map.m_map          = EnvironmentArray{ new Environment[max_line * max_col] };
   map.m_window_index = Graphics::Gui::createWindow
      (  Graphics::Gui::Window::MAIN
      ,  map.x_size + 2
      ,  map.y_size + 2
      ,  map.x_offset
      ,  map.y_offset
      );

   std::cout << " MAP INDEX ! : " << map.m_window_index << std::endl;
   
   int iline = 0;
   while(std::getline(map_file, str) && (iline < max_line))
   {
      auto ncol = std::min(max_col, int(str.size()));
      for(int i = 0; i < ncol; ++i)
      {
         map.m_map[iline + i * max_line] = Environment::create(str[i]);
      }
      ++iline;
   }
   
   return map;
}

/**
 * Check that move is ok
 **/
bool Map::isMoveOkay(const Actor& actor, int y, int x) const
{
   if(actor.noclip)
   {
      return true;
   }

   if((y < 0) || (y >= y_size) || (x < 0) || (x >= x_size))
   {
      return false;
   }

   if(actor.flying)
   {
      return true;
   }

   if(!m_map[y + y_size * x].passable)
   {
      return false;
   }

   return true;
}

/**
 * Draw map to framebuffer
 **/
void Map::draw() const
{
   auto window = Graphics::Gui::getWindow(m_window_index)->getWindow();
   
   for(int x = 0; x < x_size; ++x)
   {
      for(int y = 0; y < y_size; ++y)
      {
         m_map[y + y_size * x].draw(window, x, y);
      }
   }
}

} /* namespace game */
