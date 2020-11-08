#include "JSON.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <fstream>
#include <iostream>
#include <string>
#include "FileSystem.hpp"
#include "Utils.hpp"

std::string cleanJSONValue(const std::string& value) {
  if (value[value.length() - 1] == ',') {
    return value.substr(1, value.length() - 3);
  } else {
    return value.substr(1, value.length() - 2);
  }
}

std::string getId(CustomVector<std::string>* params) {
  auto site = (*params)[params->getLength() - 2];
  auto id = (*params)[params->getLength() - 1];
  std::string fin;
  auto pointPos = id.find(".");
  auto actualId = id.substr(0, pointPos);
  fin.append(site).append("//").append(actualId);
  return fin;
}

CameraDTO* JSON::parseJSON(const std::string& path) {
  std::ifstream file(path);
  std::string line;
  auto params = Utils::splitString(path, "/");
  auto id = getId(params);
  delete params;
  auto camera = new CameraDTO();
  camera->setId(id);
  if (file.bad()) {
    std::cout << "Something horrible happend when opening the file at: " << path
              << std::endl;
    exit(EXIT_FAILURE);
  }
  while (std::getline(file, line)) {
    if (line.find("{") != std::string::npos ||
        line.find("}") != std::string::npos) {
      continue;
    }
    auto kv = Utils::splitString(line, ":");
    if (kv->getLength() != 2) {
      delete kv;
      continue;
    }
    auto key = kv->get(0);
    auto value = kv->get(1);
    delete kv;
    auto isNotArrayStart = value.find("[") == std::string::npos &&
                           value.find("]") == std::string::npos &&
                           value.find("\"") != std::string::npos;
    auto isArrayStart = value.find("[") != std::string::npos &&
                        value.find("\"") == std::string::npos;
    if (isNotArrayStart) {
      camera->addProperty(key, value);
    } else if (isArrayStart) {
      auto array = new CustomVector<std::string>(10);
      std::getline(file, line);
      auto isArrayEnd = line.find("]") != std::string::npos && value.find("\"") == std::string::npos;
      while (!isArrayEnd) {
        array->add(line);
        std::getline(file, line);
        isArrayEnd = line.find("]") != std::string::npos && value.find("\"") == std::string::npos;
      }
      camera->addProperty(key, array);
    }
  }

  file.close();

  return camera;
}

CustomVector<CameraDTO*>* JSON::loadData(const std::string& basePath) {
  auto files = FileSystem::getAllFiles(basePath);
  auto cameras = new CustomVector<CameraDTO*>(5000);
  auto count = 0;
  for (auto i = files->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto current = i->getValue();
    auto camera = JSON::parseJSON(current);
    cameras->add(camera);
    // std::cout << "Added camera #" << count++ << std::endl;
  }
  delete files;
  return cameras;
}