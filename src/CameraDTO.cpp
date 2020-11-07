#include "CameraDTO.hpp"

#include <string>

CameraDTO::CameraDTO() {
  this->properties = new HashMap<CameraProperty*>();
}

void CameraDTO::setTitle(std::string title) {
  this->title = title;
}

void CameraDTO::addProperty(std::string name, std::string value) {
  auto prop = new CameraProperty(value);
  this->properties->set(name, prop);
}

void CameraDTO::addProperty(std::string name, std::string* value) {
  auto prop = new CameraProperty(value);
  this->properties->set(name, prop);
}

void CameraDTO::setId(std::string id) {
  this->id = id;
}

std::string CameraDTO::getId() {
  return this->id;
}

HashMap<CameraProperty*>* CameraDTO::getProperties() {
  return this->properties;
}

CameraDTO::~CameraDTO() {
  auto allProps = this->properties->getEntries();
  for (auto i = allProps->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto val = i->getValue();
    delete val->value;
    delete val;
  }
  delete allProps;
  delete this->properties;
}

CameraProperty::CameraProperty(std::string value) {
  this->setValue(value);
}

CameraProperty::CameraProperty(std::string* value) {
  this->setValue(value);
}

CameraProperty::~CameraProperty() {
  if (this->arrayValue != nullptr) {
    delete[] this->arrayValue;
  }
}

void CameraProperty::setValue(std::string value) {
  this->value = value;
  this->isArray = false;
}

void CameraProperty::setValue(std::string* value) {
  this->arrayValue = value;
  this->isArray = true;
}