#include "simulated_execution_handler.h"

#include <cassert>

SimulatedExecutionHandler::SimulatedExecutionHandler()  {}

void SimulatedExecutionHandler::handleOrder(OrderEvent oe) {
  std::string ticker = oe.ticker;

  // get existing order / create new empty order
  PendingOrder& order = pending_orders_[ticker];
  order.last_order_ts = oe.bar.start_ts;


  if (order.quantity == 0) {
    // just insert new order
    order.quantity = oe.mkt_quantity;
    order.order_side = oe.order_side;
  } else {
    // update existing order
    if (order.order_side == BUY && oe.order_side == SELL) {
      int diff = static_cast<int>(order.order_side) - static_cast<int>(oe.order_side);
      order.quantity = static_cast<std::size_t>(std::abs(diff));

      if (diff < 0) {
        // Order now selling
        order.order_side = SELL;
      }
    } else if (order.order_side == SELL && oe.order_side == BUY) {
      int diff = -static_cast<int>(order.order_side) + static_cast<int>(oe.order_side);
      order.quantity = static_cast<std::size_t>(std::abs(diff));

      if (diff > 0) {
        // Order now buying
        order.order_side = BUY;
      }
    } else {
      //BUY && BUY or SELL && SELL
      order.quantity += oe.mkt_quantity;
    }
  }
}

FillEvent SimulatedExecutionHandler::onMarket(MarketEvent market) {
  std::string ticker = market.bar.ticker;
  std::chrono::sys_time<std::chrono::milliseconds> new_bar_ts = market.bar.start_ts;

  // find pending order
  FillEvent empty{ticker, 0, 0, BUY};
  auto it = pending_orders_.find(ticker);
  if (it == pending_orders_.end()) {
    return empty;
  }

  PendingOrder& pending_order = pending_orders_[ticker];
  if (pending_order.quantity == 0) {
    return empty;
  }
  
  if (!(new_bar_ts > pending_order.last_order_ts)) {
    return empty;
  }

  double price = market.bar.open;
  // fill full order
  FillEvent filled{ticker, pending_order.quantity, price, pending_order.order_side};
  pending_orders_.erase(ticker); 

  return filled;
}

