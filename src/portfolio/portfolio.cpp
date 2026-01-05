#include "portfolio.h"

#include <iostream>
#include <cassert>

Portfolio::Portfolio(double cash) : cash_(cash), realized_pnl_(0)  {}

OrderEvent Portfolio::handle_signal(SignalEvent signal) {
  std::size_t order_amnt = 1;
  std::string ticker = signal.ticker;

  switch (signal.direction) {
    case LONG:
      return OrderEvent{ticker, order_amnt, BUY, signal.bar};
    case SHORT:
      return OrderEvent{ticker, order_amnt, SELL, signal.bar};
    case EXIT: {
      int cur_pos = positions_[signal.ticker].quantity;
      std::size_t pos_amount = static_cast<std::size_t>(std::abs(cur_pos));
      if (cur_pos < 0) 
        return OrderEvent{ticker, pos_amount, BUY, signal.bar};
      else
        return OrderEvent{ticker, pos_amount, SELL, signal.bar};
      }
    default:
      assert (false && "handle_signal unreachable");
  }
}

void Portfolio::on_fill(FillEvent fill) {
  std::string ticker = fill.ticker;
  std::size_t quantity = fill.quantity;
  double fill_price = fill.fill_price;
  OrderSide order_side = fill.order_side;

  this->record(ticker, quantity, fill_price, order_side);
  history_.push_back(fill);
}

void Portfolio::record(std::string ticker, std::size_t quantity, 
                       double fill_price, OrderSide order_side) {
  if (quantity == 0) {
    //dont do anything
    return;
  }

  std::string sign;
  if (order_side == BUY) {
    sign = "";
  } else {
    sign = "-";
  }

  std::cout << "ORDER FILLED: " << sign <<
    quantity << "@" << fill_price << std::endl;

  auto& pos = positions_[ticker];

  if (order_side == SELL) {
    //SELLING
    int sell_qty = quantity;
    if (pos.quantity <= 0) {
      //shorting more / opening short position
      pos.quantity -= sell_qty;
      pos.cost_basis -= sell_qty * fill_price;
    } else {
      //process current long position. open new short position if necessary.
      double avg_pos_price = pos.cost_basis / pos.quantity; //will be positive
      
      int selling_amount = std::min(pos.quantity, sell_qty);
      int shorting_amount = std::abs(std::min(0, pos.quantity - sell_qty));

      pos.quantity -= selling_amount;
      pos.cost_basis -= selling_amount * avg_pos_price;
      realized_pnl_ += (fill_price - avg_pos_price)*selling_amount;

      //if sold more than owned, short position opened.
      if (shorting_amount > 0) {
        //open new short position
        pos.quantity -= shorting_amount;
        pos.cost_basis -= shorting_amount * fill_price;
      }
    }
    cash_ += sell_qty * fill_price;
  } else {
    //BUYING
    int buy_qty = quantity;
    if (pos.quantity >= 0) {
      //opening/extending long position
      pos.quantity += buy_qty;
      pos.cost_basis += buy_qty * fill_price;
    } else {
      //SHORT POSITION HELD
      //process current short position. open long position if necessary
      double avg_pos_price = pos.cost_basis / pos.quantity;

      int closing_short_amount = std::min(buy_qty, -pos.quantity);
      int long_amount = std::max(0, buy_qty + pos.quantity);

      pos.quantity += closing_short_amount;
      pos.cost_basis += closing_short_amount * avg_pos_price;
      realized_pnl_ += (avg_pos_price - fill_price)*closing_short_amount;

      //if bought more than shorted, long position opened;
      if (long_amount > 0) {
        pos.quantity += long_amount;
        pos.cost_basis += long_amount * fill_price;
      }
    }
    cash_ -= buy_qty * fill_price;
  }

  if (pos.quantity == 0) {
    positions_.erase(ticker);
  }

  std::cout << "[cash/realized/position: " << cash_ << ", " << realized_pnl_ 
    << ", " << pos.quantity << "(" << pos.cost_basis << ")" << "]" << std::endl;
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
