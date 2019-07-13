#pragma once
#ifndef QUEST_UTIL_SPLIT_STRING_HPP_INCLUDED 
#define QUEST_UTIL_SPLIT_STRING_HPP_INCLUDED 

#include <string>
#include <vector>

namespace Util
{

std::vector<std::string> splitString(const std::string& str, const std::string& delimeter = " ")
{
   size_t pos_start = 0, pos_end, delim_len = delimeter.length();
   std::string              token;
   std::vector<std::string> res;

   while ((pos_end = str.find (delimeter, pos_start)) != std::string::npos) 
   {
       token = str.substr (pos_start, pos_end - pos_start);
       pos_start = pos_end + delim_len;
       res.push_back (token);
   }

   res.push_back (str.substr (pos_start));
   return res;
}

} /* namespace Util */

#endif  /* QUEST_UTIL_SPLIT_STRING_HPP_INCLUDED */
