#pragma once

#include <map>

#include "istrategy.h"


class BuyEverythingStrategy : public IStrategy {
public:
  BuyEverythingStrategy(std::string);

  Decision onBar(const Bar &bar) override;

private:
  std::string ticker_to_buy_;
  std::map<std::string, std::pair<int, double>> bought_tickers_;
};
