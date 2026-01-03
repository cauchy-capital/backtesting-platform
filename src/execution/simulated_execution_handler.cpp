#include "simulated_execution_handler.h"

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
  return FillEvent{};
}

