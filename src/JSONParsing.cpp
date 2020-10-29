#include "JSONParsing.hpp"
#include "Utils.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>

CameraDTO* JSONParsing::parseJSON(const char* path, const char* filename) {
  FILE* fp;
  char* line = NULL;
  size_t len = 0;
  
  fp = fopen(path, "r");
  if (fp == NULL) {
    printf("Welp something was not okie dokie, errno: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  bool finished = false;

  auto camera = new CameraDTO();

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

List<CameraDTO*>* JSONParsing::loadData(const char* basePath) {
  
}