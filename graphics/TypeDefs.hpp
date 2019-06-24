#pragma once
#ifndef QUEST_GRAPHICS_TYPEDEFS_HPP_INCLUDED
#define QUEST_GRAPHICS_TYPEDEFS_HPP_INCLUDED

#include <curses.h>

#define GRASS    ' '
#define EMPTY    ' '
#define WATER	  '~'
#define MOUNTAIN '^'
#define PLAYER	  '@'
#define GRAIL    'G'
#define ACTOR    'A'
#define TREE     '*'
#define TORCH     '*'
#define BINARY_ZERO ' '
#define BINARY_ONE ' '
#define ORC        'O'
#define WALL       '#'

#define DEFAULT_COLOR 1
#define PLAYER_COLOR 2
#define GRAIL_COLOR  3
#define ENEMY_COLOR  4
#define WATER_COLOR  5
#define SMALL_WATER_COLOR  6
#define TREE_COLOR  7
#define BINARY_ZERO_COLOR 8
#define BINARY_ONE_COLOR  9
#define TORCH_COLOR  10
#define WALL_COLOR  11

#endif /* QUEST_GRAPHICS_TYPEDEFS_HPP_INCLUDED */
