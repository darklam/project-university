#include "CameraDTO.hpp"

CameraDTO::CameraDTO() {
  this->properties = new HashMap<CameraProperty>(false);
  this->title = nullptr;
  this->id = nullptr;
}

void CameraDTO::setTitle(char* title) {
  this->title = title;
}

void CameraDTO::addProperty(char* name, char* value) {
  auto prop = new CameraProperty(value);
  this->properties->set(name, prop);
}

void CameraDTO::addProperty(char* name, char** value) {
  auto prop = new CameraProperty(value);
  this->properties->set(name, prop);
}

void CameraDTO::setId(char* id) {
  this->id = id;
}

char* CameraDTO::getId() {
  return this->id;
}

HashMap<CameraProperty>* CameraDTO::getProperties() {
  return this->properties;
}

CameraDTO::~CameraDTO() {
  if (this->title != nullptr) {
    delete[] this->title;
  }
  if (this->id != nullptr) {
    delete[] this->id;
  }

  delete this->properties;
}

CameraProperty::CameraProperty(char* value) {
  this->setValue(value);
}

CameraProperty::CameraProperty(char** value) {
  this->setValue(value);
}

void CameraProperty::setValue(char* value) {
  this->value = value;
  this->isArray = false;
}

void CameraProperty::setValue(char** value) {
  this->arrayValue = value;
  this->isArray = true;
}