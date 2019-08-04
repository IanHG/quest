#include "Map.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cmath>

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
      return GenerateDungeon(70, 40, 1);
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
 *
 **/
bool Map::isSpecialMapExit(const Actor& actor, int x, int y) const
{
   return m_map[y + this->y_size * x].exit;
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

int midPoint(int x1, int x2)
{
   if(x1 > x2)
   {
      return int(x2 + (x1 - x2) / 2);
   }
   else
   {
      return int(x1 + (x2 - x1) / 2);
   }
};

/**
 *
 **/
Map GenerateDungeon(int map_x_size, int map_y_size, int floor_level)
{
   /**
    * Some local structs and functions
    **/
   struct Point
   {
      int x;
      int y;
   };

   enum class Type     : int { Room, NSCon, EWCon };
   enum class ConnType : int { None, N, S, E, W, NW, NE, SW, SE };

   struct Property
   {
      enum Type : int { None, HasKey, HasEntry, HasExit, LockedDoor };

      Type type;
   };

   struct Room
   {
      // Top left
      Point l;
      // Bottom right
      Point r;

      Type  type      = Type::Room;
      bool  connected = false;
      bool  draw      = true;
      int   depth     = 0;
      int   index     = -1;
      
      Property properties[10] = { {Property::None} };
      bool  has_key   = false;
      bool  is_entry  = false;
      bool  is_exit   = false;

      // connection specific
      int   from        = -1;
      int   to          = -1;
      bool  door_locked = false;
   
      // Add property to room, returns 'true' if property was added, otherwise returns 'false'
      bool addProperty(const Property& property)
      {
         for(int i = 0; i < 10; ++i)
         {
            if(properties[i].type == Property::None)
            {
               properties[i] = property;
               return true;
            }
         }
         return false;
      }
   };

   struct Range
   {
      int min;
      int max;
   };
   
   auto checkRoomOverlap = [](const Room& room, const std::vector<Room>& rooms){
      auto check = [](const Room& room1, const Room& room2)
      {
         //bool noOverlap =  room1.l.x > room2.r.x ||
         //                  room2.l.x > room1.r.x ||
         //                  room1.l.y > room2.r.y ||
         //                  room2.l.y > room1.r.y;
         
         bool noOverlap =  room1.l.x - 1 > room2.r.x + 1 ||
                           room2.l.x - 1 > room1.r.x + 1 ||
                           room1.l.y - 1 > room2.r.y + 1 ||
                           room2.l.y - 1 > room1.r.y + 1;

         return !noOverlap;
      };

      for(int i = 0; i < int(rooms.size()); ++i)
      {
         auto& other_room = rooms[i];
         if(&room == &other_room)
         {
            continue;
         }
         if (check(room, other_room) && check(other_room, room))
         {
            return true;
         }
      }

      return false;
   };

   auto drawRoom = [](const Room& room, Map& map){
      if(!room.draw)
      {
         return;
      }

      for(int y = room.l.y; y <= room.r.y; ++y)
      {
         map.m_map[y + map.y_size * (room.l.x)] = Environment::create('#');
         map.m_map[y + map.y_size * (room.r.x)] = Environment::create('#');
      }
      for(int x = room.l.x; x <= room.r.x; ++x)
      {
         map.m_map[(room.l.y) + map.y_size * x] = Environment::create('#');
         map.m_map[(room.r.y) + map.y_size * x] = Environment::create('#');
      }

      switch(room.type)
      {
         case Type::NSCon:
         {
            int x = room.l.x + (room.r.x - room.l.x) / 2;
            map.m_map[room.l.y + map.y_size * x] = Environment::create(' ');
            map.m_map[room.r.y + map.y_size * x] = Environment::create(' ');
            int door_index = midPoint(room.l.y, room.r.y) + map.y_size * midPoint(room.l.x, room.r.x);
            if(room.door_locked)
            {
               map.m_map[door_index].sprite = Graphics::getSprite(Graphics::Sprite::DoorLockedHorizontal);
            }
            else
            {
               map.m_map[door_index].sprite = Graphics::getSprite(Graphics::Sprite::DoorHorizontal);
            }
            break;
         }
         case Type::EWCon:
         {
            int y = room.l.y + (room.r.y - room.l.y) / 2;
            map.m_map[y + map.y_size * room.l.x] = Environment::create(' ');
            map.m_map[y + map.y_size * room.r.x] = Environment::create(' ');
            int door_index = midPoint(room.l.y, room.r.y) + map.y_size * midPoint(room.l.x, room.r.x);
            if(room.door_locked)
            {
               map.m_map[door_index].sprite = Graphics::getSprite(Graphics::Sprite::DoorLockedVertical);
            }
            else
            {
               map.m_map[door_index].sprite = Graphics::getSprite(Graphics::Sprite::DoorVertical);
            }
            break;
         }
         case Type::Room:
         {
            switch(room.index) 
            {
               case 0:
                  map.m_map[room.l.y + map.y_size * room.l.x].sprite = Graphics::getSprite(Graphics::Sprite::Zero);
                  break;
               case 1:
                  map.m_map[room.l.y + map.y_size * room.l.x].sprite = Graphics::getSprite(Graphics::Sprite::One);
                  break;
               case 2:
                  map.m_map[room.l.y + map.y_size * room.l.x].sprite = Graphics::getSprite(Graphics::Sprite::Two);
                  break;
               case 3:
                  map.m_map[room.l.y + map.y_size * room.l.x].sprite = Graphics::getSprite(Graphics::Sprite::Three);
                  break;
            }
            if(room.has_key)
            {
               Graphics::Gui::instance->message("creating key \n");
               map.m_map[room.l.y + map.y_size * room.l.x].sprite = Graphics::getSprite(Graphics::Sprite::Grail);
            }
            if(room.is_entry)
            {
               int roll_x = rollDieRange(room.l.x + 1, room.r.x - 1);
               int roll_y = rollDieRange(room.l.y + 1, room.r.y - 1);

               map.x_entry = roll_x;
               map.y_entry = roll_y;
               
               map.m_map[roll_y + map.y_size * roll_x].sprite = Graphics::getSprite(Graphics::Sprite::DungeonStairsUp);
            }
            if(room.is_exit)
            {
               int roll_x = rollDieRange(room.l.x + 1, room.r.x - 1);
               int roll_y = rollDieRange(room.l.y + 1, room.r.y - 1);
            
               map.m_map[roll_y + map.y_size * roll_x].sprite = Graphics::getSprite(Graphics::Sprite::DungeonStairsDown);
               map.m_map[roll_y + map.y_size * roll_x].exit   = true;
            }
         }
         default:
         {
            break;
         }
      }
   };

   auto findXyCoord = [&checkRoomOverlap](Room& room, const Map& map, const std::vector<Room>& rooms, const std::vector<Room>& conns){
      auto findXCoord = [&map](){
         return rollDieRange(0, map.x_size - 1);
      };
      auto findYCoord = [&map](){
         return rollDieRange(0, map.y_size - 1);
      };
      
      room.l.x = findXCoord();
      room.l.y = findYCoord();
      int x_size  = std::min(rollDieRange(5, map.x_size / 3), map.x_size - room.l.x - 1);
      int y_size  = std::min(rollDieRange(x_size - 5, x_size - 3), map.y_size - room.l.y - 1);

      if((x_size < 5) || (y_size < 5))
      {
         return false;
      }

      room.r.x = room.l.x + x_size;
      room.r.y = room.l.y + y_size;

      if(checkRoomOverlap(room, rooms) || checkRoomOverlap(room, conns))
      {
         return false;
      }

      return true;
   };

   auto allConnected = [](const std::vector<Room>& rooms)
   {
      for(int i = 0; i < int(rooms.size()); ++i)
      {
         if(!rooms[i].connected)
         {
            return false;
         }
      }
      return true;
   };

   auto findClosest = [](const std::vector<Room>& rooms, int& room_index, int& room_index_closest, ConnType& type)
   {
      auto distance = [](const Point& point1, const Point& point2){
         int diff_x = point1.x - point2.x;
         int diff_y = point1.y - point2.y;
         return std::sqrt(diff_x * diff_x + diff_y * diff_y);
      };

      auto calculateDistance = [&distance](const Room& room1, const Room& room2, ConnType& type){
         assert(room1.connected);
         assert(!room2.connected);

         //std::stringstream sstr;
         
         // These are a little bit of a dirty hack
         bool left   = room2.r.x <= room1.l.x + 1;
         bool right  = room1.r.x <= room2.l.x + 1;

         bool top    = room2.r.y <= room1.l.y + 1;
         bool bottom = room1.r.y <= room2.l.y + 1;

         //sstr  << " ROOM 1 " << room1.index 
         //      << " " << room1.l.x << " " << room1.l.y
         //      << " " << room1.r.x << " " << room1.r.y
         //      << std::endl;
         //sstr  << " ROOM 2 " << room2.index 
         //      << " " << room2.l.x << " " << room2.l.y
         //      << " " << room2.r.x << " " << room2.r.y
         //      << std::endl;
         //sstr  << std::boolalpha 
         //      << "   L : " << left 
         //      << "   R : " << right
         //      << "   T : " << top
         //      << "   B : " << bottom << std::endl;

         //Graphics::Gui::instance->message(sstr.str());
         
         if (top && left) {
            type = ConnType::NW;
            return distance(Point{room1.l.x, room1.r.y}, Point{room2.r.x, room2.l.y});
         } else if (left && bottom) {
            type = ConnType::SW;
            return distance(Point{room1.l.x, room1.l.y}, Point{room2.r.x, room2.r.y});
         } else if (bottom && right) {
            type = ConnType::SE;
            return distance(Point{room1.r.x, room1.l.y}, Point{room2.l.x, room2.r.y});
         } else if (right && top) {
            type = ConnType::NE;
            return distance(Point{room1.r.x, room1.r.y}, Point{room2.l.x, room2.l.y});
         } else if (left) {
            type = ConnType::W;
            return double(room1.l.x - room2.r.x);
         } else if (right) {
            type = ConnType::E;
            return double(room2.l.x - room1.r.x);
         } else if (bottom) {
            type = ConnType::S;
            return double(room2.r.y - room1.l.y);
         } else if (top) {
            type = ConnType::N;
            return double(room1.l.y - room2.r.y);
         } else {
            return 0.0;
         }
      };

      //room_index            = 0;
      //room_index_closest    = 0;
      double distance_value = -1.0;
      
      for(int i = 0; i < int(rooms.size()); ++i)
      {
         if(!rooms[i].connected)
         {
            for(int j = 0; j < int(rooms.size()); ++j)
            {
               if(rooms[j].connected)
               {
                  ConnType type_new;
                  auto distance_value_new = calculateDistance(rooms[j], rooms[i], type_new);
                  //if(type == ConnType::N || type == ConnType::S || type == ConnType::E || type == ConnType::W)
                  //{
                  if(distance_value_new <= distance_value || (distance_value == -1.0))
                  {
                     room_index         = j;
                     room_index_closest = i;
                     distance_value     = distance_value_new;
                     type               = type_new;
                  }
                  //}
               }
            }
         }
      }
   };

   auto createConnection = [](std::vector<Room>& rooms, std::vector<Room>& conns, int room_index, int room_index_closest, ConnType type)
   {

      auto& room         = rooms[room_index];
      auto& room_connect = rooms[room_index_closest];
      
      bool connection_created = false;

      Room connection;
      connection.connected = true;
      connection.from      = room.index;
      connection.to        = room_connect.index;
      if(type == ConnType::N)
      {
         //Graphics::Gui::instance->message("N " + std::to_string(room.index) + " " + std::to_string(room_connect.index) + "\n");
         connection.type    = Type::NSCon;
         Range intersection = { std::max(room.l.x, room_connect.l.x), std::min(room.r.x, room_connect.r.x) };
         connection.l       = Point{midPoint(intersection.min, intersection.max) - 1, room_connect.r.y};
         connection.r       = Point{midPoint(intersection.min, intersection.max) + 1, room.l.y};
         connection_created = true;
      }
      else if(type == ConnType::S)
      {
         //Graphics::Gui::instance->message("S " + std::to_string(room.index) + " " + std::to_string(room_connect.index) + "\n");
         connection.type    = Type::NSCon;
         Range intersection = { std::max(room.l.x, room_connect.l.x), std::min(room.r.x, room_connect.r.x) };
         connection.l       = Point{midPoint(intersection.min, intersection.max) - 1, room.r.y};
         connection.r       = Point{midPoint(intersection.min, intersection.max) + 1, room_connect.l.y};
         connection_created = true;
      }
      else if(type == ConnType::E)
      {
         //Graphics::Gui::instance->message("E " + std::to_string(room.index) + " " + std::to_string(room_connect.index) + "\n");
         connection.type = Type::EWCon;
         Range intersection = { std::max(room.l.y, room_connect.l.y), std::min(room.r.y, room_connect.r.y) };
         connection.l       = Point{room.r.x,         midPoint(intersection.min, intersection.max) - 1};
         connection.r       = Point{room_connect.l.x, midPoint(intersection.min, intersection.max) + 1};
         connection_created = true;
      }
      else if(type == ConnType::W)
      {
         //Graphics::Gui::instance->message("W " + std::to_string(room.index) + " " + std::to_string(room_connect.index) + "\n");
         connection.type = Type::EWCon;
         Range intersection = { std::max(room.l.y, room_connect.l.y), std::min(room.r.y, room_connect.r.y) };
         connection.l       = Point{room_connect.r.x, midPoint(intersection.min, intersection.max) - 1};
         connection.r       = Point{room.l.x        , midPoint(intersection.min, intersection.max) + 1};
         connection_created = true;
      }
      
      if(connection_created)
      {
         room_connect.depth     = room.depth + 1;
         room_connect.connected = true;
         conns.emplace_back(connection);
      }

      //conns.emplace_back(connection);

   };

   // Create map
   Map map = Map::create(map_x_size, map_y_size);
   
   std::vector<Room> rooms;
   std::vector<Room> conns;
   
   // Create rooms
   // 
   // Make some weighing algorithm, that will try to place rooms in less dense areas
   for(int i = 0; i < 50; ++i)
   {
      Room room;
      int tries = 0;
      bool success = false;
      while((tries != 100) && !(success = findXyCoord(room, map, rooms, conns)))
      {
         ++tries;
      };
      if(success)
      {
         room.index = rooms.size();
         rooms.emplace_back(room);
      }
   }

   // Connect rooms / make doors
   rooms[0].connected = true;
   int tries = 0;
   while(!allConnected(rooms) && (tries != 100))
   {  
      ConnType type          = ConnType::None;
      int room_index         = 0;
      int room_index_closest = 0;
      findClosest(rooms, room_index, room_index_closest, type);
      if(type != ConnType::None)
      {
         createConnection(rooms, conns, room_index, room_index_closest, type);
      }
      ++tries;
   }

   for(int i = 0; i < int(rooms.size()); ++i)
   {
      if(!rooms[i].connected)
      {
         rooms[i].draw = false;
      }
   }

   // Lock some doors
   for(int i = 0; i < int(conns.size()); ++i)
   {
      auto roll = rollDie(10);
      if(roll >= 5)
      {
         auto depth = rooms[conns[i].to].depth - 1;
         auto room_roll = rollDie(int(rooms.size()));
         int j = room_roll; 
         do
         {
            if(rooms[j].connected && !rooms[j].has_key && (rooms[j].depth <= depth))
            {
               rooms[j].has_key     = true;
               conns[i].door_locked = true;
               break;
            }
            ++j;
         } while(j % int(rooms.size()) != room_roll);
      }
   }

   // Lock some doors with levers
   
   // Place entry and exit
   // Place entry in rooms[0]
   rooms[0].is_entry = true;

   // Place exit if not last floor
   if(floor_level < WorldMap::y_size)
   {
      // Do breadth first search for last conected room and place exit there
      int depth = 0;
      int index = 0;
      for(int i = 0; i < int(rooms.size()); ++i)
      {
         if(rooms[i].depth > depth)
         {
            index = i;
            depth = rooms[i].depth;
         }
      }
      rooms[index].is_exit = true;
   }
   
   // Draw
   for(int i = 0; i < int(rooms.size()); ++i)
   {
      drawRoom(rooms[i], map);
   }
   for(int i = 0; i < int(conns.size()); ++i)
   {
      drawRoom(conns[i], map);
   }

   
   // Spawn monsters and traps and create chests
   // Place chest with frequency scaling with number connections (Low connection == High chest percentage)
   // Higher percentage if behind locked doors ??
   for(int i = 0; i < int(rooms.size()); ++i)
   {
   }

   // Place traps
   
   // Place monsters (higher percentage in rooms with chests)
   
   return map;
}

} /* namespace game */
