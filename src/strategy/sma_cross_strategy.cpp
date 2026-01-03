#include <stdexcept>


#include "sma_cross_strategy.h"
#include "../backtest/signal_event.h"


SmaCrossStrategy::SmaCrossStrategy(std::string ticker,
                                   std::size_t short_window,
                                   std::size_t long_window,
                                   int trade_qty)
  : ticker_(std::move(ticker)),
    short_window_(short_window),
    long_window_(long_window),
    trade_qty_(trade_qty) {
  if (short_window_ == 0 || long_window_ == 0) {
    throw std::invalid_argument("SMA windows must be > 0");
  }
  if (short_window_ >= long_window_) {
    throw std::invalid_argument("short_window must be < long_window");
  }
  if (trade_qty_ <= 0) {
    throw std::invalid_argument("trade_qty must be > 0");
  }
}

double SmaCrossStrategy::sma_last_n_(std::size_t n) const {
  // assumes closes_.size() >= n
  double sum = 0.0;
  auto it = closes_.rbegin();
  for (std::size_t i = 0; i < n; ++i, ++it) {
    sum += *it;
  }
  return sum / static_cast<double>(n);
}

SignalEvent SmaCrossStrategy::onBar(const MarketEvent& e) {
  Bar bar = e.bar;

  // If you want to strictly trade only the configured ticker:
  if (!ticker_.empty() && bar.ticker != ticker_) {
    return SignalEvent{bar.ticker, NOACT, bar};
  }

  // Use close as the signal/decision price (you can swap to bar.open, bar.low, etc.).
  const double price = bar.close;

  // Update rolling window of closes
  closes_.push_back(price);
  if (closes_.size() > long_window_) {
    closes_.pop_front();
  }

  // Need enough history to compute long SMA
  if (closes_.size() < long_window_) {
    return SignalEvent{bar.ticker, NOACT, bar};
  }

  const double short_sma = sma_last_n_(short_window_);
  const double long_sma  = sma_last_n_(long_window_);

  // First bar where SMAs are valid: store prev and do nothing
  if (!has_prev_sma_) {
    has_prev_sma_ = true;
    prev_short_sma_ = short_sma;
    prev_long_sma_  = long_sma;
    return SignalEvent{bar.ticker, NOACT, bar};
  }

  const bool crossed_up =
      (prev_short_sma_ <= prev_long_sma_) && (short_sma > long_sma);

  const bool crossed_down =
      (prev_short_sma_ >= prev_long_sma_) && (short_sma < long_sma);

  // Update prev values for next bar
  prev_short_sma_ = short_sma;
  prev_long_sma_  = long_sma;

  if (!has_position_ && crossed_up) {
    has_position_ = true;
    return SignalEvent{bar.ticker, LONG, bar};
  }

  if (has_position_ && crossed_down) {
    has_position_ = false;
    return SignalEvent{bar.ticker, SHORT, bar};

  }

  return SignalEvent{bar.ticker, NOACT, bar};
}

