#ifndef UTILS_H
#define UTILS_H

#include "List.hpp"

namespace Utils {
  List<std::string>* splitString(std::string str, std::string delimiter);
}

#endif