#include "JSON.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include "FileSystem.hpp"
#include "Utils.hpp"

std::string cleanJSONValue(const std::string& value) {
  if (value[value.length() - 1] == ',') {
    return value.substr(1, value.length() - 3);
  } else {
    return value.substr(1, value.length() - 2);
  }
}

std::string getId(FastVector<std::string>& params) {
  auto site = params[params.getLength() - 2];
  auto id = params[params.getLength() - 1];
  std::string fin;
  auto pointPos = id.find(".");
  auto actualId = id.substr(0, pointPos);
  fin.append(site).append("//").append(actualId);
  return fin;
}

CameraDTO* JSON::parseJSON(const std::string& path) {
  FILE* file = fopen(path.c_str(), "r");
  size_t len = 0;
  if (!file) {
    std::cout << "Something went wrong when opening the file " << path
              << std::endl;
    perror("Error");
    exit(EXIT_FAILURE);
  }
  FastVector<std::string> tokens(5);
  Utils::splitString(path, "/", tokens);
  auto id = getId(tokens);
  auto camera = new CameraDTO();
  camera->setId(id);
  bool finished = false;
  char* ln = nullptr;
  while (!finished) {
    int length = getline(&ln, &len, file);
    if (length == -1) {
      finished = true;
      continue;
    }
    std::string line(ln);
    if (line.find("{") != std::string::npos ||
        line.find("}") != std::string::npos) {
      continue;
    }
    FastVector<std::string> kv(6);
    Utils::splitString(line, ":", kv);
    if (kv.getLength() != 2) {
      continue;
    }
    auto key = kv[0];
    auto value = kv[1];
    auto isNotArrayStart = value.find("[") == std::string::npos &&
                           value.find("]") == std::string::npos &&
                           value.find("\"") != std::string::npos;
    auto isArrayStart = value.find("[") != std::string::npos &&
                        value.find("\"") == std::string::npos;
    if (isNotArrayStart) {
      camera->addProperty(key, value);
    } else if (isArrayStart) {
      auto array = new CustomVector<std::string>(10);
      int length = getline(&ln, &len, file);
      if (length == -1) {
        finished = true;
        continue;
      }
      line = std::string(ln);
      auto isArrayEnd = line.find("]") != std::string::npos &&
                        value.find("\"") == std::string::npos;
      while (!isArrayEnd) {
        array->add(line);
        getline(&ln, &len, file);
        if (length == -1) {
          finished = true;
          break;
        }
        line = std::string(ln);
        isArrayEnd = line.find("]") != std::string::npos &&
                     value.find("\"") == std::string::npos;
      }
      camera->addProperty(key, array);
    }
  }
  if (ln != nullptr) {
    free(ln);
  }
  fclose(file);

  return camera;
}

void JSON::loadData(const std::string& basePath,
                    FastVector<CameraDTO*>& cameras) {
  FastVector<std::string> files(30000);
  FileSystem::getAllFiles(basePath, files);
  auto useThreads = Utils::getEnvVar("USE_THREADS");
  if (useThreads == "1") {
    auto coreCount = std::thread::hardware_concurrency();
    std::thread handles[coreCount];
    std::mutex m;
    for (int core = 0; core < coreCount; core++) {
      handles[core] = std::thread([&files, &coreCount, core, &cameras, &m]() {
        int start, end;
        Utils::getBatchIndex(&start, &end, files.getLength(), coreCount, core);
        for (int i = start; i < end; i++) {
          auto current = files[i];
          auto camera = JSON::parseJSON(current);
          m.lock();
          cameras.append(camera);
          m.unlock();
        }
      });
    }
    for (int i = 0; i < coreCount; i++) {
      handles[i].join();
    }
  } else {
    for (int i = 0; i < files.getLength(); i++) {
      auto current = files[i];
      auto camera = JSON::parseJSON(current);
      cameras.append(camera);
    }
  }
}