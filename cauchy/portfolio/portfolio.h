#pragma once

#include <vector>
#include <unordered_map>

#include <cauchy/core/events/signal_event.h>
#include <cauchy/core/events/order_event.h>
#include <cauchy/core/events/fill_event.h>
#include <cauchy/core/order_side.h>

struct Position {
  int quantity;
  double cost_basis; //cumulative amount spent on current holdings
};

/*
 * Portfolio
 *
 * Maintains the strategy’s current account state: cash balance, open positions,
 * and fill history. The portfolio is updated exclusively via fills (executions),
 * and can translate strategy signals into concrete orders.
 *
 * Responsibilities:
 * - Order generation: handle_signal() converts a SignalEvent into an OrderEvent
 *   according to the portfolio’s current holdings/cash and the signal direction.
 * - State updates: on_fill() applies a FillEvent to update positions, cost basis,
 *   cash, realized PnL, and records the fill in history_.
 * - PnL reporting: get_pnl() returns total PnL (realized + unrealized) using a
 *   provided map of latest prices for marking open positions to market.
 *
 * Notes on accounting:
 * - positions_ tracks per-ticker Position { quantity, cost_basis }, where
 *   cost_basis represents the cumulative amount paid for the currently-held
 *   quantity (used to compute realized/unrealized PnL depending on fills).
 * - realized_pnl_ accumulates profits/losses from closed portions of positions.
 * - unrealized PnL is computed by marking open positions to the provided prices.
 */

class Portfolio {
 public:
  Portfolio(double cash);

  OrderEvent handle_signal(SignalEvent signal);

  void on_fill(FillEvent fill);

  double get_pnl(const std::unordered_map<std::string, double> prices) const;

 private:
  void record(std::string ticker, std::size_t quantity, double fill_price, OrderSide order_side);
  double unrealized_pnl(const std::unordered_map<std::string, double> prices) const;

  std::unordered_map<std::string, Position> positions_; 
  std::vector<FillEvent> history_;
  double cash_;  
  double initial_holdings_;
  double realized_pnl_;
};

