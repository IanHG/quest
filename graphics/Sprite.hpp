#pragma once 
#ifndef QUEST_GRAPHICS_SPRITE_HPP_INCLUDED
#define QUEST_GRAPHICS_SPRITE_HPP_INCLUDED

#include "TypeDefs.hpp"
#include "Graphics.hpp"

#include <cassert>
#include <vector>
#include <memory>

namespace Graphics
{

/**
 *
 **/
struct Sprite
{
   enum : int
   {  Empty  = 0
   ,  Player
   ,  Grail
   ,  Mountain
   ,  SmallMountain
   ,  Water
   ,  ShallowWater
   ,  Tree
   ,  SmallTree
   ,  Torch
   ,  Wall
   ,  BinaryZero
   ,  BinaryOne
   ,  Orc
   ,  Rock
   ,  Plate
   ,  Door
   ,  DoorVertical
   ,  DoorHorizontal
   ,  DoorLockedVertical
   ,  DoorLockedHorizontal
   ,  Chest
   ,  LeverLeft
   ,  LeverRight
   ,  DungeonStairsUp
   ,  DungeonStairsDown
   ,  Zero
   ,  One
   ,  Two
   ,  Three
   ,  Error
   ,  Size
   };

   CharType symbol = 'E';
   int      color  = 0;
};

///**
// *
// **/
//struct SpriteProxy
//{
//   const Sprite* m_sprite = nullptr;
//
//   SpriteProxy(const Sprite* const sprite)
//      :  m_sprite(sprite)
//   {
//   }
//
//   SpriteProxy(SpriteProxy&& other)
//      :  m_sprite(other.m_sprite)
//   {
//   }
//
//   SpriteProxy& operator=(SpriteProxy&& other)
//   {
//      m_sprite = other.m_sprite;
//      return *this;
//   }
//
//   const Sprite* const operator->() const
//   {
//      return m_sprite;
//   }
//};

/**
 *
 **/
struct SpriteContainer
{
   using container_type = std::vector<Sprite>;
   using code_type      = std::vector<char>;

   container_type m_container = container_type{Sprite::Size};
   code_type      m_code;
   
   /**
    * Constructor
    **/
   SpriteContainer()
   {
      m_container[Sprite::Empty]         = Sprite{' ', color_pairs[COLOR::BLACK][COLOR::BLACK]};
      m_container[Sprite::Player]        = Sprite{'@', color_pairs[COLOR_CYAN][COLOR_BLACK]};
      m_container[Sprite::Grail]         = Sprite{'Y', color_pairs[COLOR::YELLOW][COLOR::BLACK]};
      m_container[Sprite::Mountain]      = Sprite{'^', color_pairs[COLOR::WHITE][COLOR::RED]};
      m_container[Sprite::SmallMountain] = Sprite{'^', color_pairs[COLOR::RED][COLOR::BLACK]};
      m_container[Sprite::Water]         = Sprite{'~', color_pairs[COLOR::CYAN][COLOR::BLUE]};
      m_container[Sprite::ShallowWater]  = Sprite{'~', color_pairs[COLOR::BLUE][COLOR::BLACK]};
      m_container[Sprite::Tree]          = Sprite{'*', color_pairs[COLOR::RED][COLOR::GREEN]};
      m_container[Sprite::SmallTree]     = Sprite{'*', color_pairs[COLOR::GREEN][COLOR::BLACK]};
      m_container[Sprite::Torch]         = Sprite{'*', color_pairs[COLOR::YELLOW][COLOR::BLACK]};
      m_container[Sprite::Wall]          = Sprite{'#', color_pairs[COLOR::WHITE][COLOR::RED]};
      m_container[Sprite::BinaryZero]    = Sprite{' ', color_pairs[COLOR::BLACK][COLOR::DARKER_RED]};
      m_container[Sprite::BinaryOne]     = Sprite{' ', color_pairs[COLOR::BLACK][COLOR::BLACK]};
      m_container[Sprite::Orc]           = Sprite{'O', color_pairs[COLOR::RED][COLOR::BLACK]};
      m_container[Sprite::Rock]          = Sprite{'0', color_pairs[COLOR::CYAN][COLOR::BLACK]};
      m_container[Sprite::Plate]         = Sprite{' ', color_pairs[COLOR::WHITE][COLOR::RED]};
      m_container[Sprite::Door]          = Sprite{'D', color_pairs[COLOR::WHITE][COLOR::RED]};
      m_container[Sprite::DoorVertical]  = Sprite{'|', color_pairs[COLOR::WHITE][COLOR::BLACK]};
      m_container[Sprite::DoorHorizontal]= Sprite{'_', color_pairs[COLOR::WHITE][COLOR::BLACK]};
      m_container[Sprite::DoorLockedVertical]  = Sprite{'|', color_pairs[COLOR::YELLOW][COLOR::BLACK]};
      m_container[Sprite::DoorLockedHorizontal]= Sprite{'_', color_pairs[COLOR::YELLOW][COLOR::BLACK]};
      m_container[Sprite::DungeonStairsUp]   = Sprite{'<', color_pairs[COLOR::WHITE][COLOR::BLACK]};
      m_container[Sprite::DungeonStairsDown] = Sprite{'>', color_pairs[COLOR::WHITE][COLOR::BLACK]};
      m_container[Sprite::LeverLeft]     = Sprite{'\\', color_pairs[COLOR::CYAN][COLOR::BLACK]};
      m_container[Sprite::LeverRight]    = Sprite{'/', color_pairs[COLOR::CYAN][COLOR::BLACK]};
      m_container[Sprite::Chest]         = Sprite{'?', color_pairs[COLOR::YELLOW][COLOR::BLACK]};
      m_container[Sprite::Zero]          = Sprite{'0', color_pairs[COLOR::YELLOW][COLOR::BLACK]};
      m_container[Sprite::One]           = Sprite{'1', color_pairs[COLOR::YELLOW][COLOR::BLACK]};
      m_container[Sprite::Two]           = Sprite{'2', color_pairs[COLOR::YELLOW][COLOR::BLACK]};
      m_container[Sprite::Three]         = Sprite{'3', color_pairs[COLOR::YELLOW][COLOR::BLACK]};
      m_container[Sprite::Error]         = Sprite{'E', color_pairs[COLOR::BLUE][COLOR::RED]};
      
      //std::string braille_msg = 
      // " ⠁⠂⠃⠄⠅⠆⠇⠈⠉⠊⠋⠌⠍⠎⠏\n"
      // "⠐⠑⠒⠓⠔⠕⠖⠗⠘⠙⠚⠛⠜⠝⠞⠟\n"
      // "⠠⠡⠢⠣⠤⠥⠦⠧⠨⠩⠪⠫⠬⠭⠮⠯\n"
      // "⠰⠱⠲⠳⠴⠵⠶⠷⠸⠹⠺⠻⠼⠽⠾⠿\n";
      //
   }

   /**
    * Get sprite 
    **/
   const Sprite* getSprite(int sprite_index) const
   {
      if(sprite_index < int(m_container.size()) && sprite_index >= 0)
      {
         return &(m_container[sprite_index]);
      }
      return &(m_container[0]);
   }
   
   /**
    * Initialize/create sprite container
    **/
   static void create()
   {
      instance = std::unique_ptr<SpriteContainer>{ new SpriteContainer };
   }
   
   // Pointer to sprite container instance
   static std::unique_ptr<SpriteContainer> instance;
};

inline std::unique_ptr<SpriteContainer> SpriteContainer::instance = std::unique_ptr<SpriteContainer>{nullptr};

inline auto getSprite(char c)
{
   return SpriteContainer::instance->getSprite(c);
}

} /* namespace Graphics */

#endif /* QUEST_GRAPHICS_SPRITE_HPP_INCLUDED */
