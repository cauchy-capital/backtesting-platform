#pragma once

#include <cauchy/core/events/market_event.h>
#include <cauchy/core/events/signal_event.h>

// Interface for strategy. traders should implement onBar()
class IStrategy {
public:
  virtual ~IStrategy() = default;
  virtual SignalEvent onBar(const MarketEvent& market_event) = 0;
};
