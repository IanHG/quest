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
      /* Initialize colors 
       * param 1     : color name
       * param 2, 3, 4 : rgb content min = 0, max = 1000 
       */
      //init_color(COLOR::DARK_RED, 500, 0, 0);
      //init_color(COLOR::DARKER_RED, 300, 0, 0);
      //init_color(COLOR::LIGHT_RED, 1000, 300, 0);
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
