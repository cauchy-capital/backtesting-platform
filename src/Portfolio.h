#pragma once

#include <vector>
#include <unordered_map>

#include "Decision.h"

class Portfolio {
 public:
  Portfolio(double cash);

  void record(const Decision& decision);

  double calculate_pnl() const;

 private:
  std::unordered_map<std::string, std::pair<int, double>> holdings_;
  std::vector<Decision> history_;
  double cash_;  
};

