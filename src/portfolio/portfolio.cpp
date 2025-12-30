#include "portfolio.h"

#include <iostream>

Portfolio::Portfolio(double cash) : cash_(cash), realized_pnl_(0)  {}

void Portfolio::record(const Decision& decision) {
  if (decision.quantity == 0) {
    //dont do anything
    return;
  }
  std::cout << "DECISION: " << decision.quantity << "@" << decision.price << std::endl;

  auto& pos = positions_[decision.ticker];

  if (decision.quantity < 0) {
    //SELLING
    int sell_qty = -decision.quantity;
    if (pos.quantity <= 0) {
      //shorting more / opening short position
      pos.quantity -= sell_qty;
      pos.cost_basis -= sell_qty * decision.price;
    } else {
      //process current long position. open new short position if necessary.
      double avg_pos_price = pos.cost_basis / pos.quantity; //will be positive
      
      int selling_amount = std::min(pos.quantity, sell_qty);
      int shorting_amount = std::abs(std::min(0, pos.quantity - sell_qty));

      pos.quantity -= selling_amount;
      pos.cost_basis -= selling_amount * avg_pos_price;
      realized_pnl_ += (decision.price - avg_pos_price)*selling_amount;

      //if sold more than owned, short position opened.
      if (shorting_amount > 0) {
        //open new short position
        pos.quantity -= shorting_amount;
        pos.cost_basis -= shorting_amount * decision.price;
      }
    }
    cash_ += sell_qty * decision.price;
  } else {
    //BUYING
    int buy_qty = decision.quantity;
    if (pos.quantity >= 0) {
      //opening/extending long position
      pos.quantity += buy_qty;
      pos.cost_basis += buy_qty * decision.price;
    } else {
      //SHORT POSITION HELD
      //process current short position. open long position if necessary
      double avg_pos_price = pos.cost_basis / pos.quantity;

      int closing_short_amount = std::min(buy_qty, -pos.quantity);
      int long_amount = std::max(0, buy_qty + pos.quantity);

      pos.quantity += closing_short_amount;
      pos.cost_basis += closing_short_amount * avg_pos_price;
      realized_pnl_ += (avg_pos_price - decision.price)*closing_short_amount;

      //if bought more than shorted, long position opened;
      if (long_amount > 0) {
        pos.quantity += long_amount;
        pos.cost_basis += long_amount * decision.price;
      }
    }
    cash_ -= buy_qty * decision.price;
  }
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
  std::cout << "realized_pnl: " << realized_pnl_ << "\n unrealized_pnl: " << this->unrealized_pnl(prices) << std::endl;
  return realized_pnl_ + this->unrealized_pnl(prices);
}
