#pragma once

#include <vector>
#include <string>
#include "IDataFeed.h"
#include "../Bar.h"

class CsvDataFeed : public IDataFeed {
public:
  CsvDataFeed(std::string);

  std::vector<Quote> loadData() override;

private:
  std::string filepath_;
};
