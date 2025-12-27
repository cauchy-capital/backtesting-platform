#include "portfolio.h"

#include <iostream>

Portfolio::Portfolio(double cash) : cash_(cash)  {}

void Portfolio::record(const Decision& decision) {
  if (decision.quantity == 0) {
    //dont do anything
    return;
  }
  std::cout << "DECISION: " << decision.quantity << "@" << decision.price << std::endl;

  auto& pos = positions_[decision.ticker];
  double notional = decision.quantity * decision.price;

  if (decision.quantity < 0) {
    //SELL
    //calculate pnl
    double sell_qty = -decision.quantity;

    double avg_before = pos.cost_basis / pos.quantity;
    double pnl = (decision.price - avg_before) * sell_qty;

    realized_pnl_ += pnl;
  }

  //update holdings
  cash_ -= notional;
  pos.quantity += decision.quantity;
  pos.cost_basis += notional;

  history_.push_back(decision);
}

double Portfolio::unrealized_pnl(const std::unordered_map<std::string, double> prices) const {
  double upnl = 0.0;
  for (const std::pair<const std::string, Position>& e : positions_) {
    std::string ticker = e.first;
    Position p = e.second;

    auto it = prices.find(ticker);
    if (it == prices.end()) {
      continue;
    }

    //calculate pnl
    double price = it->second;
    double avg_cost = p.cost_basis / p.quantity;
    upnl += (price - avg_cost) * p.quantity;
  }
  return upnl;
}

double Portfolio::get_pnl(const std::unordered_map<std::string, double> prices) const {
  return realized_pnl_ + this->unrealized_pnl(prices);
}
