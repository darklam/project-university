#ifndef UTILS_H
#define UTILS_H

#include "CustomVector.hpp"
#include <string>

namespace Utils {
  CustomVector<std::string>* splitString(const std::string& str, const std::string& delimiter);
  void splitStringLite(const std::string&, const std::string&, std::string*, int );
}

#endif