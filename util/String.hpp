#pragma once
#ifndef QUEST_UTIL_SPLIT_STRING_HPP_INCLUDED 
#define QUEST_UTIL_SPLIT_STRING_HPP_INCLUDED 

#include <string>
#include <vector>

namespace Util
{

/**
 * Split a string
 **/
std::vector<std::string> splitString(const std::string& str, const std::string& delimeter = " ");

/**
 * Trim a string
 **/
std::string ltrim(const std::string& s);
std::string rtrim(const std::string& s);
std::string trim(const std::string& s);

} /* namespace Util */

#endif  /* QUEST_UTIL_SPLIT_STRING_HPP_INCLUDED */
