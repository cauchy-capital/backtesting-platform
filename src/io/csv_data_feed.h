#pragma once

#include <vector>
#include <string>
#include <chrono>

#include "idata_feed.h"

class CsvDataFeed : public IDataFeed {
public:
  CsvDataFeed(std::string, std::string);

  std::vector<Quote> loadData() override;

private:
  std::string filepath_;
  std::string ticker_;

  std::vector<std::string> splitTab(const std::string& line);

  std::chrono::sys_time<std::chrono::milliseconds> parseTimeStamp(const std::string& s);
};
