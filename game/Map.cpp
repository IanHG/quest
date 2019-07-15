#include "Map.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "../graphics/Graphics.hpp"
#include "../graphics/Gui.hpp"
#include "../util/FromString.hpp"
#include "../util/String.hpp"
#include "Game.hpp"

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

   // Read map sizse
   std::getline(map_file, str);
   int max_line, max_col;
   std::istringstream s_str(str);
   s_str >> max_line >> max_col;
   
   map.x_size = max_col;
   map.y_size = max_line;
   
   // Create window to hold map
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
   
   // Create static elements
   int iline = 0;
   while((iline < max_line) && std::getline(map_file, str))
   {
      auto ncol = std::min(max_col, int(str.size()));
      for(int i = 0; i < ncol; ++i)
      {
         map.m_map[iline + i * max_line] = Environment::create(str[i]);
      }
      ++iline;
   }

   // Create dynamic elements
   while(std::getline(map_file, str))
   {
      Graphics::Gui::instance->message(str);
      Graphics::Gui::instance->message("\n");
      str = Util::trim(str);

      if(str.compare("active") == 0)
      {
         Graphics::Gui::instance->message("active??");
         std::getline(map_file ,str);
         int n_active = Util::fromString<int>(str);
         for(int i = 0; i < n_active; ++i)
         {
            std::getline(map_file, str);
            auto line_split = Util::splitString(str);  
            if(line_split[0] == "pressure_plate")
            {
               int x_plate = Util::fromString<int>(line_split[2]);
               int y_plate = Util::fromString<int>(line_split[3]);
               char sprite_plate = line_split[1][0];
               map.m_map[y_plate + max_line * x_plate] = Environment::create(sprite_plate);
               int x_door  = Util::fromString<int>(line_split[5]);
               int y_door  = Util::fromString<int>(line_split[6]);
               char sprite_door = line_split[4][0];
               map.m_map[y_door + max_line * x_door] = Environment::create(sprite_door);
               map.m_map[y_door + max_line * x_door].passable = false;

               map.m_map[y_plate + max_line * x_plate].move_on = [x_door, y_door, max_line](Map& map, Environment& env, Actor& actor) 
                  {
                     map.m_map[y_door + max_line * x_door].passable = true;
                  };
               map.m_map[y_plate + max_line * x_plate].move_off = [x_door, y_door, max_line](Map& map, Environment& env, Actor& actor) 
                  { 
                     map.m_map[y_door + max_line * x_door].passable = false;
                  };
            }
         }
      }
      else if(str.compare("actors") == 0)
      {
         std::getline(map_file ,str);
         int n_actors = Util::fromString<int>(str);
         Graphics::Gui::instance->message(std::to_string(n_actors));
         Graphics::Gui::instance->message("\n");
         for(int i = 0; i < n_actors; ++i)
         {
            std::getline(map_file, str);
            auto line_split = Util::splitString(str);  

            std::string actor_type = line_split[0];
            int         x_actor    = Util::fromString<int>(line_split[1]);
            int         y_actor    = Util::fromString<int>(line_split[2]);
            
            Graphics::Gui::instance->message(actor_type);
            Graphics::Gui::instance->message(" ");
            Graphics::Gui::instance->message(std::to_string(x_actor));
            Graphics::Gui::instance->message(" ");
            Graphics::Gui::instance->message(std::to_string(y_actor));
            Graphics::Gui::instance->message("\n");

            //
            int actor_index = instance->createNpc(actor_type, x_actor, y_actor);

            if(line_split.size() > 3)
            {
               // Load dialog
               auto actor = instance->getActor(actor_index);
               if(actor)
               {
               }
            }

            map.m_actors.emplace_back(actor_index);
         }
      }
      else
      {
         Graphics::Gui::instance->message("no hit :C");
         Graphics::Gui::instance->message("\n");
      }
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
         auto& sprite = m_map[y + y_size * x].sprite;
         Graphics::drawCell(window, x, y, sprite->symbol, sprite->color);
      }
   }
}

/**
 * Draw actor to framebuffer
 **/
void Map::drawActor(const Actor& actor) const
{
   auto& sprite_fg = actor.sprite;
   auto& sprite_bg = m_map[actor.y + this->y_size * actor.x].sprite;

   short int color_fg;
   short int color_bg;
   short int dummy;

   pair_content(sprite_fg->color, &color_fg, &dummy);
   pair_content(sprite_bg->color, &dummy   , &color_bg);
   
   auto window = Graphics::Gui::getWindow(m_window_index)->getWindow();
   
   Graphics::drawCell(window, actor.x, actor.y, sprite_fg->symbol, Graphics::color_pairs[color_fg][color_bg]);
}

} /* namespace game */
