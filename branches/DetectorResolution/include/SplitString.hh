#ifndef SplitString_h
#define SplitString_h 1

#include <string>
#include <sstream>
#include <vector>

namespace SS
{
   std::vector<std::string> & split(const std::string &s,
         const char &delim, std::vector<std::string> &elems)
   {
      std::stringstream ss(s);
      std::string item;
      while (std::getline(ss, item, delim))
      {
         elems.push_back(item);
      }
      return elems;
   }


   std::vector<std::string> split(const std::string &s, const char &delim)
   {
      std::vector<std::string> elems;
      elems = SS::split(s, delim, elems);
      return elems;
   }
}
#endif
