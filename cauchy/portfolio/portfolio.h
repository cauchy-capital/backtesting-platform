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

