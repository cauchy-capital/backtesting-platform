#pragma once

#include <string>
#include <chrono>

struct Quote {
  std::string ticker;
  std::chrono::sys_time<std::chrono::milliseconds> ts;
  double bid;
  double ask;
  double bidVol;
  double askVol;
};
