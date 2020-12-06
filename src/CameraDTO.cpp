#include "CameraDTO.hpp"
#include "Set.hpp"
#include <string>

void CameraDTO::setTitle(const std::string& title) {
  this->title = title;
}

void CameraDTO::addProperty(const std::string& name, const std::string& value) {
  this->properties.append(name).append(" ").append(value);
}

void CameraDTO::addProperty(const std::string& name, CustomVector<std::string>* value) {
  std::string current = name;
  Set s;
  for (int i = 0; i < value->getLength(); i++) {
    auto curr = (*value)[i];
    if (!s.exists(curr)) {
      s.add(curr);
      current.append(" ").append(curr);
    }
  }
}

void CameraDTO::setId(const std::string& id) {
  this->id = id;
}

std::string CameraDTO::getId() {
  return this->id;
}

std::string CameraDTO::getProperties() {
  return this->properties;
}

std::string CameraDTO::getAll() {
  std::string all = this->title;
  return all.append(" ").append(this->properties);
}