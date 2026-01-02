#pragma once

#include "../backtest/market_event.h"
#include "../backtest/signal_event.h"

// Interface for strategy. traders should implement onBar()
class IStrategy {
public:
  virtual ~IStrategy() = default;
  virtual SignalEvent onBar(const MarketEvent& market_event) = 0;
};
