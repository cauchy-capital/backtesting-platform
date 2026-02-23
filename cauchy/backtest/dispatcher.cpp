#include "dispatcher.h"
#include "backtester.h"

#include <iostream>

void Dispatcher::operator()(MarketEvent& me) const {
  bt.last_bar_ = me.bar;

  if (std::optional<FillEvent> fe = bt.curr_execution_handler_->onMarket(me)) {
    std::cerr << "[FILLEVENT]" << "\n";
    bt.event_queue_.push(std::move(*fe));
  }

  SignalEvent se = bt.curr_strat_->onBar(me);
  std::cerr << "[BAR] close=" << me.bar.close
          << " signal_dir=" << se.direction << "\n";
  bt.event_queue_.push(std::move(se));
}

void Dispatcher::operator()(SignalEvent& se) const {
  std::cerr << "[SIGNAL] dir=" << se.direction << "\n";
  if (se.direction == NOACT) return;

  OrderEvent oe = bt.portfolio_.handle_signal(se);
  std::cerr << "[ORDER] qty=" << oe.mkt_quantity
          << " side=" << oe.order_side << "\n";
  bt.event_queue_.push(std::move(oe));
}

void Dispatcher::operator()(OrderEvent& oe) const {
  bt.curr_execution_handler_->handleOrder(oe);
}

void Dispatcher::operator()(FillEvent& fe) const {
  bt.portfolio_.on_fill(fe);
}
