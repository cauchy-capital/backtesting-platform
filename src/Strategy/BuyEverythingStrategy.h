#pragma once

#include <map>

#include "IStrategy.h"


class BuyEverythingStrategy : public IStrategy {
public:
  BuyEverythingStrategy(std::string);

  Decision onBar(Quote quote) override;

private:
  std::string ticker_to_buy_;
  std::map<std::string, int> bought_tickers_;
};
