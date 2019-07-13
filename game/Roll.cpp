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

} /* namespace Game */
