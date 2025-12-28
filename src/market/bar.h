#pragma once

#include <string>
#include <chrono>

struct Bar {
  std::string ticker;
  std::chrono::sys_time<std::chrono::milliseconds> start_ts;
  std::chrono::sys_time<std::chrono::milliseconds> end_ts;
  double open;
  double high;
  double low;
  double close;
  double volume;
};
