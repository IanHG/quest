#pragma once
#ifndef QUEST_ENGINE_ENGINE_HPP_INCLUDED
#define QUEST_ENGINE_ENGINE_HPP_INCLUDED

#include <functional>

namespace Engine
{

/**
 *
 **/
void initialize();

/**
 *
 **/
void gameLoop(const std::function<bool()>&);

} /* namespace Engine */

#endif /* QUEST_ENGINE_ENGINE_HPP_INCLUDED */
