#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#include "CameraDTO.hpp"
#include "List.hpp"
#include "CustomVector.hpp"

namespace JSON {
  CameraDTO* parseJSON(const char* path);
  CustomVector<CameraDTO>* loadData(const char* basePath);
}

#endif