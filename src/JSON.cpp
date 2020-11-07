#include "JSON.hpp"
#include "Utils.hpp"
#include "FileSystem.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <iostream>
#include <string>

std::string getId(List<std::string>* params) {
  auto site = params->get(params->getLength() - 2);
  auto id = params->get(params->getLength() - 1);
  std::string fin;
  auto pointPos = id.find(".");
  auto actualId = id.substr(0, pointPos);
  fin.append(site).append("//").append(actualId);
  return fin;
}

CameraDTO* JSON::parseJSON(std::string path) {
  FILE* fp;
  std::string line = NULL;
  size_t len = 0;
  auto params = Utils::splitString(path, "/");
  auto id = getId(params);
  delete params;
  fp = fopen(path.c_str(), "r");
  if (fp == NULL) {
    std::cout << "Welp something was not okie dokie, errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
  bool finished = false;

  auto camera = new CameraDTO();
  camera->setId(id);

  while (!finished) {
    int length = std::getline(&line, &len, fp);
    if (length == -1) {
      finished = true;
      continue;
    }
    if (Utils::stringContains(line, "{") || Utils::stringContains(line, "}")) {
      continue;
    }
    auto kv = Utils::splitString(line, ":");
    if (kv->getLength() != 2) {
      continue;
    }
    auto key = kv->get(0);
    auto value = kv->get(1);
    if (key == nullptr || value == nullptr) {
      printf("Something really bad happened\n");
    }
    if (!(Utils::stringContains(value, "[") || Utils::stringContains(value, "]") )) {
      camera->addProperty(key, value);
    }
  }

  fclose(fp);
  if (line != NULL) {
    free(line);
  }

  return camera;
}

CustomVector<CameraDTO>* JSON::loadData(const char* basePath) {
  auto files = FileSystem::getAllFiles(basePath);
  auto cameras = new CustomVector<CameraDTO>(5000);
  for (auto i = files->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto current = i->getValue();
    auto camera = JSON::parseJSON(current);
    cameras->add(camera);
  }
  delete files;
  return cameras;
}