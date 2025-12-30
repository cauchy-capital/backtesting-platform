#include <iostream>

#include "backtester.h"
#include "../market/decision.h"
#include "../market/bar_builder.h"

Backtester::Backtester(double cash) : portfolio_(cash) {}

void Backtester::run_backtest() {
  if (!curr_feed_) {
    std::cerr << "Backtester ERROR: No feed set.\n";
    return;
  }

  std::chrono::milliseconds interval{1000};
  BarBuilder bar_builder{interval};

  std::vector<Quote> quotes = curr_feed_->loadData();
  std::vector<Bar> bars = bar_builder.buildBars(quotes);

  //run backtest
  for (Bar& bar : bars) {
    Decision decision = curr_strat_->onBar(bar);
    portfolio_.record(decision);

    last_bar_ = bar;
  }
  std::cout << "last BAR: PRICE=" << last_bar_.close << std::endl;
}

double Backtester::results() {
  // need ticker:price
  std::cout << std::endl;
  std::string ticker = last_bar_.ticker;
  return portfolio_.get_pnl({{ticker, last_bar_.low}});
}

void Backtester::set_feed(std::unique_ptr<IDataFeed> feed) {
  curr_feed_ = std::move(feed);
}

void Backtester::set_strat(std::unique_ptr<IStrategy> strat) {
  curr_strat_ = std::move(strat);
}


