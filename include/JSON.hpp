#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#include "CameraDTO.hpp"
#include "List.hpp"
#include "CustomVector.hpp"

namespace JSON {
  CameraDTO* parseJSON(const std::string& path);
  CustomVector<CameraDTO*>* loadData(const std::string& basePath);
}

#endif