#include "Map.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>

#include "../graphics/Graphics.hpp"
#include "../graphics/Gui.hpp"
#include "../util/FromString.hpp"
#include "../util/String.hpp"
#include "Game.hpp"
#include "Item.hpp"
#include "Roll.hpp"

namespace Game
{

/**
 * Load map file and return map struct.
 **/
Map Map::load(const std::string& map_name)
{
   if(map_name.empty())
   {
      return GenerateDungeon();
   }

   Map map;

   std::ifstream map_file{"maps/" + map_name};
   
   std::string str;

   // Read map sizse
   std::getline(map_file, str);
   int max_line = 0, max_col = 0;
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
                     map.m_map[y_door + max_line * x_door].sprite   = Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Plate);
                  };
               map.m_map[y_plate + max_line * x_plate].move_off = [x_door, y_door, max_line](Map& map, Environment& env, Actor& actor) 
                  { 
                     map.m_map[y_door + max_line * x_door].passable = false;
                     map.m_map[y_door + max_line * x_door].sprite   = Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Door);
                  };
            }
         }
      }
      else if(str.compare("actors") == 0)
      {
         std::getline(map_file ,str);
         int n_actors = Util::fromString<int>(str);
         for(int i = 0; i < n_actors; ++i)
         {
            std::getline(map_file, str);
            auto line_split = Util::splitString(str);  

            std::string actor_type = line_split[0];
            int         x_actor    = Util::fromString<int>(line_split[1]);
            int         y_actor    = Util::fromString<int>(line_split[2]);
            
            //
            int actor_index = instance->createNpc(actor_type, x_actor, y_actor);

            if(line_split.size() > 3)
            {
               // Load dialog
               auto actor = instance->getActor(actor_index);
               if(actor)
               {
                  if(line_split[0] == "chest")
                  {
                     Item* item = dynamic_cast<Item*>(actor);
                     for(int i = 3; i < int(line_split.size()); ++i)
                     {
                        item->addItem(InventoryItem::stringToType(line_split[i]), 1);
                     }
                  }
                  else if(line_split[0] == "lever")
                  {
                     Interactable* interactable = dynamic_cast<Interactable*>(actor);

                     interactable->interaction = [](Interactable& interactable, Actor& actor){
                        if(interactable.sprite->symbol == '/')
                        {
                           interactable.sprite = Graphics::getSprite(Graphics::Sprite::LeverLeft);
                        }
                        else
                        {
                           interactable.sprite = Graphics::getSprite(Graphics::Sprite::LeverRight);
                        }
                     };
                  }
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

Map Map::create(int x_size, int y_size)
{
   Map map;

   map.x_size = x_size;
   map.y_size = y_size;

   // Create window to hold map
   auto window = Graphics::Gui::getWindow(Graphics::Gui::Window::MAIN);

   map.x_offset = window->xMax() / 2 - map.x_size / 2;
   map.y_offset = window->yMax() / 2 - map.y_size / 2;
   
   //std::cerr << " SIZE   : " << map.x_size   << " " << map.y_size   << std::endl;
   //std::cerr << " OFFSET : " << map.x_offset << " " << map.y_offset << std::endl;
   map.m_map          = EnvironmentArray{ new Environment[map.x_size * map.y_size] };
   map.m_window_index = Graphics::Gui::createWindow
      (  Graphics::Gui::Window::MAIN
      ,  map.x_size + 2
      ,  map.y_size + 2
      ,  map.x_offset
      ,  map.y_offset
      );

   return map;
}

/**
 *
 **/
void Map::destroy()
{
   for(int i = 0; i < int(m_actors.size()); ++i)
   {
      instance->removeActor(m_actors[i]);
   }
   Graphics::Gui::destroyWindow(m_window_index);
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
 *
 **/
bool Map::isMapExit(const Actor& actor, int x, int y) const
{
   if (  (x < 0) 
      || (x >= this->x_size) 
      || (y < 0) 
      || (y >= this->y_size)
      )
   {
      return true;
   }

   return false;
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
   short int color_fg;
   short int color_bg;
   short int dummy;

   pair_content(actor.sprite->color, &color_fg, &dummy);
   pair_content(m_map[actor.y + this->y_size * actor.x].sprite->color, &dummy, &color_bg);
   
   auto window = Graphics::Gui::getWindow(m_window_index)->getWindow();
   
   Graphics::drawCell(window, actor.x, actor.y, actor.sprite->symbol, Graphics::color_pairs[color_fg][color_bg]);
}

/**
 *
 **/
WorldMap WorldMap::load(const std::string& world_map_name)
{
   WorldMap world_map;

   //strncpy(world_map.world[0][0], "default.map\0", world_map.string_size);
   //strncpy(world_map.world[1][0], "default.map\0", world_map.string_size);
   //strncpy(world_map.world[0][1], "default.map\0", world_map.string_size);

   return world_map;
}

/**
 *
 **/
Map GenerateDungeon()
{
   // Create map
   int map_x_size = 70;
   int map_y_size = 40;

   //std::cerr   << " MAP SIZE : " << (50 * 70) << std::endl
   //            << " ENV SIZE : " << sizeof(Environment) << std::endl;

   Map map = Map::create(map_x_size, map_y_size);

   // Generate rooms
   struct room_definition
   {
      int x_start;
      int y_start;
      int x_size;
      int y_size;
   };

   struct range
   {
      int begin;
      int end;
   };
   
   range x_size_range{3, map.x_size / 3};
   range y_size_range{3, map.y_size / 3};

   auto checkRoom = [&map](const room_definition& room){
      for(int y = room.y_start; y < room.y_start + room.y_size; ++y)
      {
         for(int x = room.x_start; x < room.x_start + room.x_size; ++x)
         {
            if(!map.m_map[y + map.y_size * x].empty())
            {
               return false;
            }
         }
      }
      return true;
   };

   auto paintRoom = [&map](const room_definition& room){
      for(int y = room.y_start; y < room.y_start + room.y_size; ++y)
      {
         for(int x = room.x_start; x < room.x_start + room.x_size; ++x)
         {
            map.m_map[y + map.y_size * x] = Environment::create('#');
         }
      }
   };

   auto drawRoom = [&map](const room_definition& room){
      for(int y = room.y_start; y < room.y_start + room.y_size; ++y)
      {
         map.m_map[y + map.y_size * (room.x_start)              ] = Environment::create('#');
         //std::cerr << " index : " << (y + map.y_size * (room.x_start + room.x_size - 1)) << std::endl;
         map.m_map[y + map.y_size * (room.x_start + room.x_size - 1)] = Environment::create('#');
      }
      for(int x = room.x_start + 1; x < room.x_start + room.x_size - 1; ++x)
      {
         map.m_map[room.y_start                   + map.y_size * x] = Environment::create('#');
         map.m_map[room.y_start + room.y_size - 1 + map.y_size * x] = Environment::create('#');
      }
   };

   auto findXyCoord = [&map](room_definition& room){
      auto findXCoord = [&map](){
         return rollDieRange(0, map.x_size - 1);
      };
      auto findYCoord = [&map](){
         return rollDieRange(0, map.y_size - 1);
      };
      
      room.x_start = findXCoord();
      room.y_start = findYCoord();
      room.x_size  = std::min(rollDieRange(5, map.x_size / 3), map.x_size - room.x_start - 1);
      room.y_size  = std::min(rollDieRange(5, map.y_size / 3), map.y_size - room.y_start - 1);

      if(room.x_size < 5 || room.y_size < 5)
      {
         return false;
      } 

      return true;
      
      //std::cerr   << room.x_start << " "
      //            << room.y_start << " "
      //            << room.x_size << " "
      //            << room.y_size << " "
      //            << room.x_start + room.x_size << " "
      //            << room.y_start + room.y_size << " "
      //            << std::endl;
      //Graphics::Gui::instance->message(std::to_string(room.x_start));
      //Graphics::Gui::instance->message(" ");
      //Graphics::Gui::instance->message(std::to_string(room.y_start));
      //Graphics::Gui::instance->message(" ");
      //Graphics::Gui::instance->message(std::to_string(room.x_size));
      //Graphics::Gui::instance->message(" ");
      //Graphics::Gui::instance->message(std::to_string(room.y_size));
      //Graphics::Gui::instance->message("\n");
   };

   for(int i = 0; i < 5; ++i)
   {
      room_definition room;
      while(!findXyCoord(room)) {};
      drawRoom(room);
   }

   // Connect rooms / make doors


   // Spawn monsters and create chests
   return map;
}

} /* namespace game */
