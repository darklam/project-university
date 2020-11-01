#ifndef CAMERA_DTO
#define CAMERA_DTO

#include "HashMap.hpp"

struct CameraProperty {
  bool isArray = false;
  char* value = nullptr;
  char** arrayValue = nullptr;

  CameraProperty(char* value);
  CameraProperty(char** value);
  void setValue(char* value);
  void setValue(char** value);
};

class CameraDTO {
 public:
  CameraDTO();
  ~CameraDTO();

  void setTitle(char* title);

  void addProperty(char* name, char* value);
  void addProperty(char* name, char** value);
  void setId(char* id);

  char* getId();

  HashMap<CameraProperty>* getProperties();

 private:
  char* title;
  char* id;
  HashMap<CameraProperty>* properties;
};

#endif