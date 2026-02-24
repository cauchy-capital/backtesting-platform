#include "dispatcher.h"
#include "backtester.h"

#include <iostream>

void Dispatcher::operator()(MarketEvent& me) const {
  bt.last_bar_ = me.bar;
  std::cout << "[BAR] open: " << me.bar.open << std::endl;

  if (std::optional<FillEvent> fe = bt.curr_execution_handler_->onMarket(me)) {
    bt.event_queue_.push(std::move(*fe));
  }

  SignalEvent se = bt.curr_strat_->onBar(me);
  std::cout << "[SIGNAL] direction: " << se.direction << std::endl;
  bt.event_queue_.push(std::move(se));
}

void Dispatcher::operator()(SignalEvent& se) const {
  if (se.direction == NOACT) return;

  OrderEvent oe = bt.portfolio_.handle_signal(se);
  bt.event_queue_.push(std::move(oe));
}

void Dispatcher::operator()(OrderEvent& oe) const {
  bt.curr_execution_handler_->handleOrder(oe);
}

void Dispatcher::operator()(FillEvent& fe) const {
  bt.portfolio_.on_fill(fe);
}
