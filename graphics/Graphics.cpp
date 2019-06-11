#include "Graphics.hpp"

#include "TypeDefs.hpp"
#include "Sprite.hpp"
#include "Gui.hpp"

namespace Graphics
{

/**
 * Initialize graphics
 **/
void initialize()
{
   // initialize locale to system's default:
   setlocale(LC_ALL, "");

   initscr();
   keypad(stdscr, TRUE);
   cbreak();
   noecho();
   curs_set(0);
   
   // Color setup
   if (has_colors()) 
   {
      start_color();
      init_pair(DEFAULT_COLOR, COLOR_WHITE,  COLOR_BLACK);
      init_pair(PLAYER_COLOR,  COLOR_CYAN,  COLOR_BLACK);
      init_pair(GRAIL_COLOR,   COLOR_YELLOW, COLOR_BLACK);
      init_pair(ENEMY_COLOR,   COLOR_WHITE,    COLOR_RED);
      init_pair(WATER_COLOR,   COLOR_CYAN,   COLOR_BLUE);
      init_pair(SMALL_WATER_COLOR,   COLOR_BLUE,   COLOR_BLACK);
      init_pair(TREE_COLOR,   COLOR_RED,   COLOR_GREEN);
      init_pair(BINARY_ZERO_COLOR,   COLOR_BLACK, COLOR_BLACK);
      init_pair(BINARY_ONE_COLOR ,   COLOR_BLACK, COLOR_RED);
      init_pair(TORCH_COLOR ,   COLOR_YELLOW, COLOR_BLACK);
   }
   
   clear();
   refresh();
   
   // Create gui
   Gui::create();
   Gui::instance->initialize();
   
   // Create sprites
   SpriteContainer::create();
}

/**
 * Finalize graphics
 **/
void finalize()
{
   Gui::instance->finalize();

   endwin();
}

} /* namespace Graphics */
