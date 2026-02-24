#pragma once

class Backtester;

#include <cauchy/backtest/backtester.h>
#include <cauchy/core/events/market_event.h>
#include <cauchy/core/events/event.h>
#include <cauchy/core/events/signal_event.h>

struct Dispatcher {
  Backtester& bt;

  void operator()(MarketEvent& me) const;
  void operator()(SignalEvent& se) const;
  void operator()(OrderEvent& oe) const;
  void operator()(FillEvent& fe) const;
};
