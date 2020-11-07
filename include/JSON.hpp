#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#include "CameraDTO.hpp"
#include "List.hpp"
#include "CustomVector.hpp"

namespace JSON {
  CameraDTO* parseJSON(std::string path);
  CustomVector<CameraDTO*>* loadData(std::string basePath);
}

#endif