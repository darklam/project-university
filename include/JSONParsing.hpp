#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#include "CameraDTO.hpp"
#include "List.hpp"

namespace JSONParsing {
  void parseJSON(const char* path, const char* filename);
  List<CameraDTO*>* loadData(const char* basePath);
}

#endif