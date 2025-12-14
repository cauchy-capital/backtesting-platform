#pragma once

#include <vector>
#include <string>
#include <sstream>

#include "IDataFeed.h"
#include "../Bar.h"

class CsvDataFeed : public IDataFeed {
public:
  CsvDataFeed(std::string);

  std::vector<Quote> loadData() override;

private:
  std::string filepath_;
  std::string ticker_name_ = "0005.HKHKD";

  std::vector<std::string> splitTab(const std::string& line);

  std::time_t parseTimeStamp(const std::string& s);
};
