#pragma once

#include <cauchy/core/events/market_event.h>
#include <cauchy/core/events/signal_event.h>

/*
 * Interface for trading strategies.
 *
 * A strategy consumes market data (one bar at a time) and produces a trading 
 * intent in the form of a SignalEvent.
 *
 * Traders implement onBar() to apply their logic/indicators to the incoming
 * MarketEvent and return a SignalEvent describing what to do for a given ticker:
 *   - LONG  : enter/increase a long position
 *   - SHORT : enter/increase a short position
 *   - EXIT  : exit/flatten an existing position
 *   - NOACT : take no action
 *
 * The returned SignalEvent also carries the associated Bar used to generate the
 * decision, allowing downstream components (portfolio) to size and route orders 
 */

class IStrategy {
public:
  virtual ~IStrategy() = default;
  virtual SignalEvent onBar(const MarketEvent& market_event) = 0;
};
