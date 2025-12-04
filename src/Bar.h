#pragma once

#include <ctime>

struct Quote {
  time_t ts;
  double bid;
  double ask;
  double bidVol;
  double askVol;
};
