#pragma once
#ifndef QUEST_GAME_MAP_HPP_INCLUDED
#define QUEST_GAME_MAP_HPP_INCLUDED

#include <functional>

#include "../graphics/Sprite.hpp"
#include "../graphics/Gui.hpp"
#include "../game/Actor.hpp"

namespace Game
{

struct Map;

/**
 *
 **/
struct Environment
{
   // Sprite
   //Graphics::SpriteProxy sprite = Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Empty);
   const Graphics::Sprite* sprite = Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Empty);
   
   // Environment characteristics
   bool passable = true;
   bool pushable = false;
   bool toxic    = false;
   
   // On move over
   using function_type    = std::function<void(Map&, Environment&, Actor&)>;
   function_type move_on  = [](Map& map, Environment& env, Actor& actor) {  };
   function_type move_off = [](Map& map, Environment& env, Actor& actor) {  };
   
   void onMoveOn(Map& map, Actor& actor)
   {
      this->move_on(map, *this, actor);
   }
   
   void onMoveOff(Map& map, Actor& actor)
   {
      this->move_off(map, *this, actor);
   }

   bool empty() const
   {
      return (this->sprite == Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Empty));
   }

   // Create field
   static Environment create(char c)
   {
      switch(c)
      {
         case 'D':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Door), false};
         case 'M':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Mountain), false};
         case 'W':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Water), false};
         case 'F':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Tree), false};
         case '*':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Torch), false};
         case '#':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Wall), false};
         case ' ':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Empty), true};
         case 'w':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::ShallowWater), true};
         case 'm':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::SmallMountain), true};
         case 'f':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::SmallTree), true};
         case '0':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::BinaryZero), true};
         case '1':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::BinaryOne), true};
         case 'P':
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Plate), true};
         default:
            return Environment{Graphics::SpriteContainer::instance->getSprite(Graphics::Sprite::Error), true};
      }
   }
};

/**
 * Game map
 **/
struct Map
{
   using border_window_ptr = std::unique_ptr<Graphics::BorderWindow>;
   using EnvironmentArray  = std::unique_ptr<Environment[]>;
   using WindowIndex       = Graphics::Gui::WindowIndex;

   // Size of map
   int x_size = 0;
   int y_size = 0;
   
   // Off set when drawing
   int x_offset = 0;
   int y_offset = 0;
    
   // Map
   EnvironmentArray m_map          = EnvironmentArray{nullptr};
   WindowIndex      m_window_index = Graphics::Gui::WindowIndex{0};

   std::vector<int> m_actors;

   Map()  = default;
   ~Map() = default;
   
   // Move ctor and move assignment
   Map(Map&&)            = default;
   Map& operator=(Map&&) = default;
   
   // Check move
   bool isMoveOkay(const Actor& actor, int y, int x) const;

   bool isMapExit(const Actor& actor, int x, int y) const;
   
   // Trigger enviroment move on
   void moveOn(Actor& actor)
   {
      m_map[actor.y + y_size * actor.x].onMoveOn(*this, actor);
   }
   
   // Trigger enviroment move off
   void moveOff(Actor& actor)
   {
      m_map[actor.y + y_size * actor.x].onMoveOff(*this, actor);
   }

   //
   void exitNorth(int& x_world, int& y_world) const
   {
      y_world -= 1;
   }
   void exitSouth(int& x_world, int& y_world) const
   {
      y_world += 1;
   }
   void exitEast (int& x_world, int& y_world) const
   {
      x_world += 1;
   }
   void exitWest (int& x_world, int& y_world) const
   {
      x_world -= 1;
   }

   // Draw map
   void draw() const;

   // Draw actor
   void drawActor(const Actor& actor) const;

   void destroy();

   // Load map from file
   static Map load(const std::string& map_name); //, const std::string& map_path = );

   //
   static Map create(int x_size, int y_size);
};

/**
 * World map
 **/
struct WorldMap
{
   static constexpr int x_size             = 5;
   static constexpr int y_size             = 5;
   static constexpr int string_size        = 64;
   char world[x_size][y_size][string_size] = {nullptr};

   enum Exit : int { North, South, East, West };

   int x_world = 0;
   int y_world = 0;

   std::string getMap(int x_get, int y_get) const
   {
      return std::string{world[x_get][y_get]};
   }

   static WorldMap load(const std::string& world_map_name); // , const std::string& world_map_path = "");
};

// Generate dungeon
Map GenerateDungeon(int map_x_size = 70, int map_y_size = 40);

/**
 *
 **/
struct MapGenerator
{
};

} /* namespace Game */

#endif /* QUEST_GAME_MAP_HPP_INCLUDED */
