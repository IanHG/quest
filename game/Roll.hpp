#pragma once
#ifndef QUEST_GAME_ROLL_HPP_INCLUDED
#define QUEST_GAME_ROLL_HPP_INCLUDED

namespace Game
{

void seedRolls();

int rollDie(int dice);

int rollDieRange(int begin, int end);

} /* namespace Game */

#endif /* QUEST_GAME_ROLL_HPP_INCLUDED */
