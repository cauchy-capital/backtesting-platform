#pragma once

#include <map>

#include <cauchy/strategy/istrategy.h>


class BuyEverythingStrategy : public IStrategy {
public:
  BuyEverythingStrategy(std::string);

  SignalEvent onBar(const MarketEvent &e) override;

private:
  std::string ticker_to_buy_;
  std::map<std::string, std::pair<int, double>> bought_tickers_;
};
