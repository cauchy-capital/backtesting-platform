#pragma once

#include <optional>

#include <cauchy/core/events/order_event.h>
#include <cauchy/core/events/fill_event.h>
#include <cauchy/core/events/market_event.h>

/*
 * @brief Interface for execution handlers.
 *
 * An execution handler bridges the backtest engine to an execution venue.
 * Implementations may be simulated (backtesting / paper trading) or connected to a
 * live broker.
 *
 * Contract / behavior:
 * - handleOrder() accepts an OrderEvent and updates internal state (e.g., validate,
 *   route, queue, mark as pending). It does not synchronously return fills.
 * - onMarket() is called for each MarketEvent and may produce a FillEvent for any
 *   previously accepted/pending orders based on the new market data. If no fill
 *   occurs, it returns std::nullopt.
 *
 * This design decouples order submission from fill generation: orders are submitted
 * immediately, while executions are realized later in response to market updates.
 */

class IExecutionHandler {
public:
  virtual ~IExecutionHandler() = default;
  virtual void handleOrder(OrderEvent order) = 0;
  virtual std::optional<FillEvent> onMarket(MarketEvent market) = 0;
};
