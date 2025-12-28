#pragma once

#include <vector>
#include <chrono>

#include "quote.h"
#include "bar.h"


class BarBuilder {
public:
  BarBuilder(std::chrono::milliseconds ms_interval);

  std::vector<Bar> buildBars(std::vector<Quote>& quotes);

  double calc_price(const Quote& quote);

  std::chrono::sys_time<std::chrono::milliseconds> floorMsTimestamp(
    std::chrono::sys_time<std::chrono::milliseconds> ts);

private:
  std::chrono::milliseconds ms_interval_; // In miliseconds
};


