#pragma once

#include <map>

#include "IStrategy.h"


class BuyEverythingStrategy : public IStrategy {
public:
  BuyEverythingStrategy(std::string);

  Decision onBar(Quote quote) override;

private:
  std::string ticker_to_buy_;
  std::map<std::string, std::pair<int, double>> bought_tickers_;
};
