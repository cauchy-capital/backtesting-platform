#pragma once

#include <ctime>

struct Quote {
  std::string ticker;
  time_t ts;
  double bid;
  double ask;
  double bidVol;
  double askVol;
};
