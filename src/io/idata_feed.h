#pragma once

#include <vector>
#include "../market/quote.h"

// interface for data feeds. should produce quotes. 
class IDataFeed {
public:
  virtual std::vector<Quote> loadData() = 0;
};

