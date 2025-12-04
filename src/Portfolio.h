#pragma once

#include <vector>

#include "Decision.h"

class Portfolio {
 public:
  Portfolio() = default;

  void record(const std::vector<Decision>& decisions);

  double calculate() const;

 private:
  double cash = 0.0;  // modern default member initializer
};

