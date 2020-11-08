#ifndef UTILS_H
#define UTILS_H

#include "List.hpp"
#include <string>

namespace Utils {
  List<std::string>* splitString(const std::string& str, const std::string& delimiter);
}

#endif