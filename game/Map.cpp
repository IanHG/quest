#include "map.hpp"

#include <fstream>
#include <string>

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

   x_offset = gui.main->xmax / 2 - x_size / 2;
   y_offset = gui.main->ymax / 2 - y_size / 2;

   map.m_map        = field_ptr_type   { new Environment[max_line * max_col] };
   map.m_map_window = border_window_ptr{ new border_window{gui.main->win, y_size + 2, x_size + 2, x_offset, y_offset} };
   
   int iline = 0;
   game_type aof;
   while(std::getline(map_file, str) && (iline < max_line))
   {
      auto ncol = std::min(max_col, int(str.size()));
      for(int i = 0; i < ncol; ++i)
      {
         aof = actor_or_field(str[i]);
         switch(aof)
         {
            case game_type::field:
               m_map[iline + i * max_line] = Environment::create(str[i]);
               break;
            case game_type::actor:
               m_map[iline + i * max_line] = Environment::create(' ');
               break;
         }
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
