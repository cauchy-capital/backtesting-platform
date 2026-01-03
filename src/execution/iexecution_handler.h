#pragma once

#include "../core/order_event.h"
#include "../core/fill_event.h"
#include "../core/market_event.h"

class IExecutionHandler {
public:
  virtual ~IExecutionHandler() = default;
  virtual void handleOrder(OrderEvent order) = 0;
  virtual FillEvent onMarket(MarketEvent market) = 0;
};
