#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#include "CameraDTO.hpp"
#include "List.hpp"
#include "FastVector.hpp"

namespace JSON {
  CameraDTO* parseJSON(const std::string& path);
  void loadData(const std::string& basePath, FastVector<CameraDTO*>& cameras);
}

#endif