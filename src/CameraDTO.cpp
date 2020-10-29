#include "CameraDTO.hpp"

CameraDTO::CameraDTO() {
  this->properties = new HashMap<CameraProperty*>();
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

HashMap<CameraProperty*>* CameraDTO::getProperties() {
  return this->properties;
}

CameraDTO::~CameraDTO() {
  if (this->title != nullptr) {
    delete[] this->title;
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