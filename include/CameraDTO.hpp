#ifndef CAMERA_DTO
#define CAMERA_DTO

#include "HashMap.hpp"
#include <string>

struct CameraProperty {
  bool isArray = false;
  std::string value;
  std::string* arrayValue = nullptr;

  CameraProperty(const std::string& value);
  CameraProperty(std::string* value);
  void setValue(const std::string& value);
  void setValue(std::string* value);
  ~CameraProperty();
};

class CameraDTO {
 public:
  CameraDTO();
  ~CameraDTO();

  void setTitle(const std::string& title);

  void addProperty(const std::string& name, const std::string& value);
  void addProperty(const std::string& name, std::string* value);
  void setId(const std::string& id);

  std::string getId();

  HashMap<CameraProperty*>* getProperties();

 private:
  std::string title;
  std::string id;
  HashMap<CameraProperty*>* properties;
};

#endif