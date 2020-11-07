#ifndef UTILS_H
#define UTILS_H

#include "List.hpp"
#include <string>

namespace Utils {
  List<std::string>* splitString(std::string str, std::string delimiter);
}

#endif