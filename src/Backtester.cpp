#include <iostream>

#include "Backtester.h"


Backtester::Backtester(double cash) : portfolio_(cash) {}

void Backtester::run_backtest() {
  if (!curr_feed_) {
    std::cerr << "Backtester ERROR: No feed set.\n";
    return;
  }

  std::vector<Quote> quotes = curr_feed_->loadData();

  //run backtest
  for (auto& quote : quotes) {
    Decision decision = curr_strat_->onBar(quote);
    portfolio_.record(decision);
  }
}

double Backtester::results() {
  return portfolio_.calculate_pnl();
}

void Backtester::set_feed(std::unique_ptr<IDataFeed> feed) {
  curr_feed_ = std::move(feed);
}

void Backtester::set_strat(std::unique_ptr<IStrategy> strat) {
  curr_strat_ = std::move(strat);
}


