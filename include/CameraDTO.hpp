#ifndef CAMERA_DTO
#define CAMERA_DTO

#include "HashMap.hpp"
#include <string>

class CameraDTO {
 public:
  void setTitle(const std::string& title);

  void addProperty(const std::string& name, const std::string& value);
  void addProperty(const std::string& name, CustomVector<std::string>* value);
  void setId(const std::string& id);
  std::string getAll();

  std::string getId();

  std::string getProperties();

 private:
  std::string title;
  std::string id;
  std::string properties = "";
};

#endif