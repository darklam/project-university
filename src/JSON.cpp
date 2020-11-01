#include "JSON.hpp"
#include "Utils.hpp"
#include "FileSystem.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>

char* getId(List<char>* params) {
  auto site = params->get(params->getLength() - 2);
  auto id = params->get(params->getLength() - 1);
  auto fin = Utils::append(site, id, "//");
  return fin;
}

CameraDTO* JSON::parseJSON(const char* path) {
  FILE* fp;
  char* line = NULL;
  size_t len = 0;
  char* copy;
  Utils::copyString(path, &copy);
  auto params = Utils::splitString(copy, "/");
  auto id = getId(params);
  delete params;
  delete[] copy;
  fp = fopen(path, "r");
  if (fp == NULL) {
    printf("Welp something was not okie dokie, errno: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  bool finished = false;

  auto camera = new CameraDTO();
  camera->setId(id);

  while (!finished) {
    int length = getline(&line, &len, fp);
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