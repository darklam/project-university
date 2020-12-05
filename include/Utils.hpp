#ifndef UTILS_H
#define UTILS_H

#include "CustomVector.hpp"
#include "FastVector.hpp"
#include <string>

namespace Utils {
  CustomVector<std::string>* splitString(const std::string& str, const std::string& delimiter);
  void splitStringLite(const std::string&, const std::string&, std::string*, int );
  void splitNew(const std::string& str, const std::string& delimiter, FastVector<std::string>& vec);
  std::string trimString(std::string& str);
  void makeLowercase(std::string& str);
  void lowerAndClean(std::string& str);
  void getBatchIndex(int* start, int* end, int length, int coreCount, int currentCore);
}

#endif