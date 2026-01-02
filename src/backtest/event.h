#pragma once

#include <string>

class Event {
public:
  virtual ~Event() = default;
  explicit Event(std::string type);
  std::string type;
};

