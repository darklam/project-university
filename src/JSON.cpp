#include "JSON.hpp"

#include <stdio.h>
#include <stdlib.h>

#include <cerrno>
#include <iostream>
#include <string>

#include "FileSystem.hpp"
#include "Utils.hpp"

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
  FILE* fp;
  size_t len = 0;
  auto params = Utils::splitString(path, "/");
  auto id = getId(params);
  delete params;
  fp = fopen(path.c_str(), "r");
  if (fp == NULL) {
    std::cout << "Welp something was not okie dokie, errno: " << errno
              << std::endl;
    exit(EXIT_FAILURE);
  }
  bool finished = false;

  auto camera = new CameraDTO();
  camera->setId(id);
  char* ln = nullptr;

  while (!finished) {
    int length = getline(&ln, &len, fp);
    if (length == -1) {
      finished = true;
      continue;
    }
    std::string line(ln);
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
    if (!(value.find("[") != std::string::npos ||
          value.find("]") != std::string::npos)) {
      camera->addProperty(key, value);
    }
  }

  if (ln != nullptr) {
    free(ln);
  }
  fclose(fp);

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