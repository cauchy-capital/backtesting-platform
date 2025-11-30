//
// Created by Yuvraj Singh on 30/11/2025.
//

#ifndef CAUCHYBACKTESTER_PORTFOLIO_H
#define CAUCHYBACKTESTER_PORTFOLIO_H

#include <list>

#include "Decision.h"

class Portfolio {
 public:
  Portfolio();

  void record(std::list<Decision>& decisions);

  int calculate();

 private:
  int cash;
};

#endif  // CAUCHYBACKTESTER_PORTFOLIO_H
