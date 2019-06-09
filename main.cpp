/* quest.c */

#include <curses.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <sstream>
#include <fstream>

#include "keyboard.hpp"
#include "keyboard_combo.hpp"
#include "window.hpp"
#include "sprite.hpp"

#define GRASS    ' '
#define EMPTY    ' '
#define WATER	  '~'
#define MOUNTAIN '^'
#define PLAYER	  '@'
#define GRAIL    'G'
#define ACTOR    'A'

#define DEFAULT_PAIR 1
#define PLAYER_PAIR 2
#define GRAIL_PAIR  3
#define ENEMY_PAIR  4
#define WATER_PAIR  5
#define TREE_PAIR  6

bool is_move_okay(int y, int x);
void draw_map(void);

struct border_window
{
   WINDOW* win_border = nullptr;
   WINDOW* win        = nullptr;
   int xmax           = 0;
   int ymax           = 0;

   border_window(WINDOW* parent_win, int height, int width, int  startx, int starty)
   {
      // create border window
      if(parent_win)
      {
         win_border = derwin(parent_win, height, width, starty, startx);
      }
      else
      {
         win_border = create_newwin(height, width, starty, startx);
      }
      box(win_border, 0, 0);
      wrefresh(win_border);

      // create actual window
      win        = derwin(win_border, height - 2, width - 2, 1, 1);
      wrefresh(win);

      // get max x and y
      getmaxyx(win, ymax, xmax);
   }

   ~border_window()
   {
      destroy_win(win);

	   wborder(win_border, ' ', ' ', ' ',' ',' ',' ',' ',' ');
      destroy_win(win_border);
   }

   void refresh()
   {
      wrefresh(win_border);
      wrefresh(win);
   }
};

struct gui_type
{
   using border_window_ptr = border_window*;

   border_window_ptr main;
   border_window_ptr lore;
   border_window_ptr stats;
   border_window_ptr status;

   void initialize()
   {
      int height  = LINES;
	   int width   = COLS * 0.8;
      main = border_window_ptr{ new border_window(nullptr, height, width, 0, 0) };

      int half_height  = LINES / 2 - 4;
      int status_width = COLS - width;
      lore   = border_window_ptr{ new border_window(nullptr, half_height, status_width, width, 0) };
      stats  = border_window_ptr{ new border_window(nullptr, half_height, status_width, width, half_height) };
      status = border_window_ptr{ new border_window(nullptr, LINES - 2*half_height , status_width, width, 2*half_height) };
   }

   WINDOW* get_main()
   {
      return main->win;
   }

   void message(const std::string& msg)
   {
      wattron(status->win, COLOR_PAIR(PLAYER_PAIR));
      wprintw(status->win, msg.c_str());
      wattroff(status->win, COLOR_PAIR(PLAYER_PAIR));
   }

   void finalize()
   {
      delete main;
      delete lore;
      delete status;
   }

   void refresh()
   {
      main  ->refresh();
      status->refresh();
   }
} gui;

enum game_type : int {field, actor};
      
//struct environment_type
struct field_type
{
   sprite_proxy sprite   = sprite_container::instance->get_sprite(' ');
   bool         passable = true;
   bool         pushable = false;
   bool         toxic    = false;
   
   void draw(WINDOW* win, int x, int y)
   {
      sprite->draw(win, x, y);
   }

   static field_type create(char c)
   {
      switch(c)
      {
         case 'M':
         case 'W':
         case 'F':
            return field_type{sprite_container::instance->get_sprite(c), false};
         case ' ':
         default:
            return field_type{sprite_container::instance->get_sprite(c), true};
      }
   }
};

struct actor_type
{
   int  x = 0;
   int  y = 0;
   sprite_proxy sprite = sprite_container::instance->get_sprite(' ');
   bool noclip = false;

   void draw(WINDOW* win = gui.get_main())
   {
      this->sprite->draw(win, x, y);
   }
};

game_type actor_or_field(char c)
{
   switch(c)
   {
      case 'P':
      case 'O':
         return game_type::actor;
      case 'M':
      case 'W':
      case 'F':
      case ' ':
      default:
         return game_type::field;
   }
}

struct game_map_type
{
   using border_window_ptr = typename gui_type::border_window_ptr;
   using field_ptr_type    = std::unique_ptr<field_type[]>;

   // Size of map
   int x_size = 0;
   int y_size = 0;
   
   // Off set when drawing
   int x_offset = 0;
   int y_offset = 0;
      
   std::string                   m_map_name   = std::string{"default.map"};
   std::unique_ptr<field_type[]> m_map        = field_ptr_type{nullptr};
   border_window_ptr             m_map_window = border_window_ptr{nullptr};

   game_map_type()
   {
   }

   void load_map()
   {
      std::ifstream map_file{"maps/" + m_map_name};
      std::string str;
      std::getline(map_file, str);
      int max_line, max_col;
      std::istringstream s_str(str);
      s_str >> max_line >> max_col;
      x_size = max_col;
      y_size = max_line;

      m_map        = field_ptr_type   { new field_type[max_line * max_col] };
      m_map_window = border_window_ptr{ new border_window{gui.main->win, y_size + 2, x_size + 2, 5, 5} };
      
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
                  m_map[iline + i * max_line] = field_type::create(str[i]);
                  break;
               case game_type::actor:
                  m_map[iline + i * max_line] = field_type::create(' ');
                  break;
            }
         }
         ++iline;
      }

   }
   
   ~game_map_type()
   {
   }

   bool is_move_okay(const actor_type& actor, int y, int x)
   {
      if(actor.noclip)
      {
         return true;
      }

      if((y < 0) || (y >= y_size) || (x < 0) || (x >= x_size))
      {
         return false;
      }

      if(!m_map[y + y_size * x].passable)
      {
         return false;
      }

      return true;
   }

   void draw()
   {
      for(int x = 0; x < x_size; ++x)
      {
         for(int y = 0; y < y_size; ++y)
         {
            m_map[y + y_size * x].draw(m_map_window->win, x, y);
         }
      }
   }

   void refresh()
   {
      m_map_window->refresh();
   }
};


bool is_win(const actor_type& player, const actor_type& grail)
{
   return (player.x == grail.x) && (player.y == grail.y);
}

void initialize_ncurses()
{
   // initialize locale to system's default:
   setlocale(LC_ALL, "");

   initscr();
   keypad(stdscr, TRUE);
   cbreak();
   noecho();
   curs_set(0);

   if (has_colors()) 
   {
      start_color();
      init_pair(DEFAULT_PAIR, COLOR_WHITE,  COLOR_BLACK);
      init_pair(PLAYER_PAIR,  COLOR_CYAN,  COLOR_BLACK);
      init_pair(GRAIL_PAIR,   COLOR_YELLOW, COLOR_BLACK);
      init_pair(ENEMY_PAIR,   COLOR_RED,    COLOR_BLACK);
      init_pair(WATER_PAIR,   COLOR_BLUE,   COLOR_BLACK);
      init_pair(TREE_PAIR,   COLOR_GREEN,   COLOR_BLACK);
   }
   
   clear();
   refresh();
}

void finalize_ncurses()
{
   endwin();
}

/**
 * Main function
 **/
int main(int argc, char* argv[])
{
   //std::string braille_msg = 
   // " ⠁⠂⠃⠄⠅⠆⠇⠈⠉⠊⠋⠌⠍⠎⠏\n"
   // "⠐⠑⠒⠓⠔⠕⠖⠗⠘⠙⠚⠛⠜⠝⠞⠟\n"
   // "⠠⠡⠢⠣⠤⠥⠦⠧⠨⠩⠪⠫⠬⠭⠮⠯\n"
   // "⠰⠱⠲⠳⠴⠵⠶⠷⠸⠹⠺⠻⠼⠽⠾⠿\n";

   /* initialize curses */
   initialize_ncurses();

   sprite_container::create();

   gui.initialize();

   /* initialize the quest map */
   actor_type    player{0, 6, sprite_container::instance->get_sprite('P')};
   game_map_type game_map;
   game_map.load_map();
   
   keyboard& kb = keyboard::instance();
   auto move_up = [&player, &game_map](const char&) {
      if (game_map.is_move_okay(player, player.y - 1, player.x))
      {
         player.y = player.y - 1;
      }
   };

   auto move_down = [&player, &game_map](const char&) {
	   if (game_map.is_move_okay(player, player.y + 1, player.x)) 
      {
		   player.y = player.y + 1;
	   }
   };

   auto move_left = [&player, &game_map](const char&) {
	   if (game_map.is_move_okay(player, player.y, player.x - 1)) 
      {
	      player.x = player.x - 1;
	   }
   };

   auto move_right = [&player, &game_map](const char&) {
	   if (game_map.is_move_okay(player, player.y, player.x + 1)) 
      {
		   player.x = player.x + 1;
	   }
   };

   struct exit_exception
   {
   };

   auto quit = [](const char&)
   {
      throw exit_exception{};
   };

   kb.register_event('w',    move_up);
   kb.register_event('W',    move_up);
   //kb.register_event(KEY_UP, move_up);
   kb.register_event('s',      move_down);
   kb.register_event('S',      move_down);
   //kb.register_event(KEY_DOWN, move_down);
   kb.register_event('a',      move_left);
   kb.register_event('A',      move_left);
   //kb.register_event(KEY_LEFT, move_left);
   kb.register_event('d',       move_right);
   kb.register_event('D',       move_right);
   //kb.register_event(KEY_RIGHT, move_right);
   //kb.register_event('q', quit);
   //kb.register_event('Q', quit);
   //keyboard_queue& queue = keyboard_queue::instance();
   //
   keyboard_combo kbc{std::vector<char>{'i', 'd', 'd', 'q', 'd'}, [](){
      gui.message("LOL DOOM\n");
   }, kb};
   keyboard_combo kbc2{std::vector<char>{'i', 'd', 'c', 'l', 'i', 'p'}, [&player](){
      if(!player.noclip)
      {
         gui.message("Nothing can stop me!\n");
         player.noclip = true;
      }
      else
      {
         gui.message("I'm bound by reality!\n");
         player.noclip = false;
      }
   }, kb};
   
   actor_type grail {10, 20, sprite_container::instance->get_sprite('G')};
   //grail .draw();
   //player.draw();
   
   try
   {
      /******************************************
       * SETUP
       ******************************************/
      kb.handle_noevent();
      
      //
      auto frame      = std::chrono::milliseconds(1000 / 144);
      auto next_frame = std::chrono::system_clock::now();
      auto last_frame = next_frame - frame;

      //gui.message(braille_msg);
      
      /******************************************
       * MAIN GAME LOOP
       ******************************************/
      while (true)
      {
         game_map.draw();
         player.draw(game_map.m_map_window->win);
         game_map.refresh();
         
         // Handle keyboard events
         kb.read_event();
         kb.handle_events();

         if(is_win(player, grail))
         {
            gui.message(" YOU WIN\n");
         }

         gui.refresh();
         
         //std::stringstream s_str;
         //s_str << "Time: "  // just for monitoring purposes
         //      << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - last_frame).count()
         //      << "ms\n";
         //gui.message(s_str.str());
         
         // Wait for next frame
         last_frame = next_frame;
         next_frame += frame;
         std::this_thread::sleep_until(next_frame);
      }
   }
   catch(const exit_exception& e)
   {

   }
   
   finalize_ncurses();
}
