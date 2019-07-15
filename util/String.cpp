#include "String.hpp"

#include <iostream>
#include <algorithm>

namespace Util
{

/**
 * Split string with delimeter
 **/
std::vector<std::string> splitString(const std::string& str, const std::string& delimeter)
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

const std::string WHITESPACE = " \n\r\t\f\v";

/**
 * Trim string from the left
 **/
std::string ltrim(const std::string& s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

/**
 * Trim string from right
 **/
std::string rtrim(const std::string& s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

/**
 * Trim string
 **/
std::string trim(const std::string& s)
{
	return rtrim(ltrim(s));
}

} /* namespace Util */
