#pragma once

#include <cstddef>
#include <deque>
#include <string>

#include <cauchy/strategy/istrategy.h>

// Buys when short SMA crosses above long SMA; sells when it crosses below.
// Assumes only 1 ticker is active at a time, but still guards by ticker.
class SmaCrossStrategy : public IStrategy {
public:
  SmaCrossStrategy(std::string ticker,
                   std::size_t short_window = 10,
                   std::size_t long_window  = 30,
                   int trade_qty            = 10);

  SignalEvent onBar(const MarketEvent &e) override;

private:
  double sma_last_n_(std::size_t n) const;

  std::string ticker_;
  std::size_t short_window_;
  std::size_t long_window_;
  int trade_qty_;

  std::deque<double> closes_;

  bool has_position_ = false;

  // For crossover detection (need previous SMA values)
  bool has_prev_sma_ = false;
  double prev_short_sma_ = 0.0;
  double prev_long_sma_  = 0.0;
};

