#include "Map.hpp"

#include <fstream>
#include <string>
#include <sstream>

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

   map.x_offset = Graphics::Gui::instance->main->xmax / 2 - map.x_size / 2;
   map.y_offset = Graphics::Gui::instance->main->ymax / 2 - map.y_size / 2;

   map.m_map        = field_ptr_type   { new Environment[max_line * max_col] };
   map.m_map_window = border_window_ptr{ new Graphics::BorderWindow{Graphics::Gui::instance->main->win, map.y_size + 2, map.x_size + 2, map.x_offset, map.y_offset} };
   
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
bool Map::isMoveOkay(const Actor& actor, int y, int x)
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
void Map::draw()
{
   for(int x = 0; x < x_size; ++x)
   {
      for(int y = 0; y < y_size; ++y)
      {
         m_map[y + y_size * x].draw(m_map_window->win, x, y);
      }
   }
}

/**
 * Refresh screen
 **/
void Map::refresh()
{
   m_map_window->refresh();
}

} /* namespace game */
