#pragma once

#include <string>

struct Decision {
  std::string ticker;  // e.g. "AAPL"
  int quantity;        // e.g. number of shares
  double price;
};

