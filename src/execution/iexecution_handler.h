#pragma once

#include "../core/order_event.h"
#include "../core/fill_event.h"
#include "../core/market_event.h"

// interface for execution handlers. may be simulated, or connected to real
// brokers
class IExecutionHandler {
public:
  virtual ~IExecutionHandler() = default;
  virtual void handleOrder(OrderEvent order) = 0;
  virtual FillEvent onMarket(MarketEvent market) = 0;
};
