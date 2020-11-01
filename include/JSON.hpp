#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#include "CameraDTO.hpp"
#include "List.hpp"

namespace JSON {
  CameraDTO* parseJSON(const char* path);
  List<CameraDTO*>* loadData(const char* basePath);
}

#endif