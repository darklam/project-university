#include "CameraDTO.hpp"

#include <string>

CameraDTO::CameraDTO() {
  this->properties = new HashMap<CameraProperty*>();
}

void CameraDTO::setTitle(const std::string& title) {
  this->title = title;
}

void CameraDTO::addProperty(const std::string& name, const std::string& value) {
  auto prop = new CameraProperty(value);
  this->properties->set(name, prop);
}

void CameraDTO::addProperty(const std::string& name, CustomVector<std::string>* value) {
  auto prop = new CameraProperty(value);
  this->properties->set(name, prop);
}

void CameraDTO::setId(const std::string& id) {
  this->id = id;
}

std::string CameraDTO::getId() {
  return this->id;
}

HashMap<CameraProperty*>* CameraDTO::getProperties() {
  return this->properties;
}

std::string CameraDTO::getAllProperties() {
  FastVector<Entry<CameraProperty*>*> props;
  this->properties->getEntries(props);

  std::string all = "";
  for (int i = 0; i < props.getLength(); i++) {
    auto entry = props[i];
    if (entry->value->isArray) {
      auto val = entry->value->arrayValue;
      for (int j = 0; j < val->getLength(); j++) {
        all.append((*val)[j]);
      }
    } else {
      all.append(entry->value->value);
    }

    delete entry;
  }

  return all;
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

CameraProperty::CameraProperty(const std::string& value) {
  this->setValue(value);
}

CameraProperty::CameraProperty(CustomVector<std::string>* value) {
  this->setValue(value);
}

CameraProperty::~CameraProperty() {
  if (this->arrayValue != nullptr) {
    delete this->arrayValue;
  }
}

void CameraProperty::setValue(const std::string& value) {
  this->value = value;
  this->isArray = false;
}

void CameraProperty::setValue(CustomVector<std::string>* value) {
  this->arrayValue = value;
  this->isArray = true;
}
