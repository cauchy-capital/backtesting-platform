#pragma once

#include <optional>

#include <cauchy/core/events/order_event.h>
#include <cauchy/core/events/fill_event.h>
#include <cauchy/core/events/market_event.h>

// interface for execution handlers. may be simulated, or connected to real
// brokers
class IExecutionHandler {
public:
  virtual ~IExecutionHandler() = default;
  virtual void handleOrder(OrderEvent order) = 0;
  virtual std::optional<FillEvent> onMarket(MarketEvent market) = 0;
};
