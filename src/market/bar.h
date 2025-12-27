#pragma once

#include <string>
#include <ctime>

struct Bar {
  std::string ticker;
  time_t start_ts;
  time_t end_ts;
  double open;
  double high;
  double low;
  double close;
  double volume;
};
