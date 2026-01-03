#pragma once

#include <vector>
#include <unordered_map>

#include "../market/decision.h"
#include "../backtest/signal_event.h"
#include "../backtest/order_event.h"

struct Position {
  int quantity;
  double cost_basis; //cumulative amount spent on current holdings
};

class Portfolio {
 public:
  Portfolio(double cash);

  OrderEvent handle_signal(SignalEvent signal);

  void record(const Decision& decision);

  double unrealized_pnl(const std::unordered_map<std::string, double> prices) const;

  double get_pnl(const std::unordered_map<std::string, double> prices) const;

 private:
  std::unordered_map<std::string, Position> positions_; 
  std::vector<Decision> history_;
  double cash_;  
  double initial_holdings_;
  double realized_pnl_;
};

