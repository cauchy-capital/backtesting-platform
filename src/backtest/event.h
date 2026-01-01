#pragma once

#include <string>

class Event {
public:
  Event(std::string type);

private:
  std::string type_;
};

