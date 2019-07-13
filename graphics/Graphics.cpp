#include "Graphics.hpp"

#include "TypeDefs.hpp"
#include "Sprite.hpp"
#include "Gui.hpp"

namespace Graphics
{

int color_pairs[COLOR::SIZE][COLOR::SIZE];

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
      //init_pair(DEFAULT_COLOR, COLOR_WHITE,  COLOR_BLACK);
      //init_pair(PLAYER_COLOR,  COLOR_CYAN,  COLOR_BLACK);
      //init_pair(GRAIL_COLOR,   COLOR_YELLOW, COLOR_BLACK);
      //init_pair(ENEMY_COLOR,   COLOR_WHITE,    COLOR_RED);
      //init_pair(WATER_COLOR,   COLOR_CYAN,   COLOR_BLUE);
      //init_pair(SMALL_WATER_COLOR,   COLOR_BLUE,   COLOR_BLACK);
      //init_pair(TREE_COLOR,   COLOR_RED,   COLOR_GREEN);
      //init_pair(BINARY_ZERO_COLOR,   COLOR_BLACK, COLOR_BLACK);
      //init_pair(BINARY_ONE_COLOR ,   COLOR_BLACK, COLOR_RED);
      //init_pair(TORCH_COLOR ,   COLOR_YELLOW, COLOR_BLACK);
      //init_pair(WALL_COLOR ,   COLOR_WHITE, COLOR_RED);
      int pair = 0;
      for(int fg = COLOR::BLACK; fg < COLOR::SIZE; ++fg)
      {
         for(int bg = COLOR::BLACK; bg < COLOR::SIZE; ++bg)
         {
            color_pairs[fg][bg] = pair;
            init_pair(color_pairs[fg][bg], fg, bg);
            ++pair;
         }
      }
   }
   else
   {
      assert(false);
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

/**
 *
 **/
void drawCell(WINDOW* win, int x, int y, char symbol, int color_pair)
{
   wattron (win, COLOR_PAIR(color_pair));
   mvwaddch(win, y, x, symbol);
   wattroff(win, COLOR_PAIR(color_pair));
}

} /* namespace Graphics */
