//
// Created by Yuvraj Singh on 30/11/2025.
//

#ifndef CAUCHYBACKTESTER_PORTFOLIO_H
#define CAUCHYBACKTESTER_PORTFOLIO_H

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

#endif  // CAUCHYBACKTESTER_PORTFOLIO_H
