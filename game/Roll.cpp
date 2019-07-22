#include "Roll.hpp"

#include <time.h>
#include <stdlib.h>

namespace Game
{

void seedRolls()
{
   srand(time(NULL));
}

int rollDie(int dice)
{
   return rand() % dice;
}

int rollDieRange(int begin, int end)
{
   return rand() % (end - begin) + begin;
}

} /* namespace Game */
