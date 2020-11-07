#ifndef CAMERA_DTO
#define CAMERA_DTO

#include "HashMap.hpp"
#include <string>

struct CameraProperty {
  bool isArray = false;
  std::string value = nullptr;
  std::string* arrayValue = nullptr;

  CameraProperty(std::string value);
  CameraProperty(std::string* value);
  void setValue(std::string value);
  void setValue(std::string* value);
};

class CameraDTO {
 public:
  CameraDTO();
  ~CameraDTO();

  void setTitle(std::string title);

  void addProperty(std::string name, std::string value);
  void addProperty(std::string name, std::string* value);
  void setId(std::string id);

  std::string getId();

  HashMap<CameraProperty*>* getProperties();

 private:
  std::string title;
  std::string id;
  HashMap<CameraProperty*>* properties;
};

#endif